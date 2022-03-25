// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/InventoryComponent.h"

#include "PortfolioCharacter.h"
#include "PortfolioPlayerController.h"
#include "PortfolioPlayerState.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "Net/UnrealNetwork.h"
#include "Datas/StructData.h"
#include "Items/Equipments/EquipmentsAmmo.h"
#include "Items/Equipments/EquipmentsEquipment.h"
#include "Items/Equipments/EquipmentsWeaponAcc.h"
#include "Kismet/KismetMathLibrary.h"

UInventoryComponent::UInventoryComponent()
{
	//////////////////////////////////////////////////////////////////////////
	// Get DT_Item_Weapon
	const FString ItemWeaponDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Weapon.DT_Item_Weapon");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Weapon(*ItemWeaponDataPath);
	check(DT_Item_Weapon.Succeeded());
	ItemWeaponTable = DT_Item_Weapon.Object;
	check(ItemWeaponTable->GetRowMap().Num() > 0);

	//////////////////////////////////////////////////////////////////////////
	// Get DT_Item_WeaponAcc
	const FString ItemWeaponAccDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_WeaponAcc.DT_Item_WeaponAcc");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_WeaponAcc(*ItemWeaponAccDataPath);
	check(DT_Item_WeaponAcc.Succeeded());
	ItemWeaponAccTable = DT_Item_WeaponAcc.Object;
	check(ItemWeaponAccTable->GetRowMap().Num() > 0);

	//////////////////////////////////////////////////////////////////////////
	// Get DT_Item_Ammo
	const FString ItemAmmoDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Ammo.DT_Item_Ammo");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Ammo(*ItemAmmoDataPath);
	check(DT_Item_Ammo.Succeeded());
	ItemAmmoTable = DT_Item_Ammo.Object;
	check(ItemAmmoTable->GetRowMap().Num() > 0);

	//////////////////////////////////////////////////////////////////////////
	// Get DT_Item_Health
	const FString ItemHealthDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Health.DT_Item_Health");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Health(*ItemHealthDataPath);
	check(DT_Item_Health.Succeeded());
	ItemHealthTable = DT_Item_Health.Object;
	check(ItemHealthTable->GetRowMap().Num() > 0);
	
	//////////////////////////////////////////////////////////////////////////
	// Get DT_Item_Boost
	const FString ItemBoostDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Boost.DT_Item_Boost");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Boost(*ItemBoostDataPath);
	check(DT_Item_Boost.Succeeded());
	ItemBoostTable = DT_Item_Boost.Object;
	check(ItemBoostTable->GetRowMap().Num() > 0);

	//////////////////////////////////////////////////////////////////////////
	// Get DT_Item_Equipment
	const FString ItemEquipmentsDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Equipment.DT_Item_Equipment");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Equipment(*ItemEquipmentsDataPath);
	check(DT_Item_Equipment.Succeeded());
	ItemEquipmentsTable = DT_Item_Equipment.Object;
	check(ItemEquipmentsTable->GetRowMap().Num() > 0);

	//////////////////////////////////////////////////////////////////////////
	// Get DT_Item_Equipment
	const FString ItemClothesDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Clothes.DT_Item_Clothes");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Clothes(*ItemClothesDataPath);
	check(DT_Item_Clothes.Succeeded());
	ItemClothesTable = DT_Item_Clothes.Object;
	check(ItemClothesTable->GetRowMap().Num() > 0);

	BackpackCapacity = 150;
	WeaponSlot = 0;
}

bool UInventoryComponent::AddBackpackItem(AItemPickupBase* PointItem)
{
	switch (PointItem->ItemType)
	{
	case EItemType::EIT_Weapon :
		{
			//auto Datas = ItemWeaponTable->FindRow<FItemWeapon>(PointItem->ID,TEXT(""));
			if (WeaponSlot >= 2)
			{
				GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("무기 %d개 있음 "), WeaponSlot ));
				return false;
			}
			else
			{
				ItemsInBackpack.Add(PointItem);
				WeaponSlot += 1;
				break;
			}
		}
	case EItemType::EIT_Accesories :
		{
			auto Datas = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(PointItem->ID,TEXT(""));
			if (TotalWeight+Datas->Weight > BackpackCapacity)
			{
				int32 Weight = TotalWeight+Datas->Weight;
				GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("가방 무게(%d) 초과 : %d"), BackpackCapacity , Weight)); 
				return false;
			}
			else
			{
				TotalWeight += Datas->Weight;
				ItemsInBackpack.Add(PointItem);
			}
			break;
		}
	case EItemType::EIT_Ammo :
		{
			auto Datas = ItemAmmoTable->FindRow<FItemAmmo>(PointItem->ID,TEXT(""));
			if (TotalWeight+(Datas->Weight*Datas->PickupAmmo) > BackpackCapacity)
			{
				int32 Weight = TotalWeight+(Datas->Weight*Datas->PickupAmmo);
				GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("가방 무게(%d) 초과 : %d"), BackpackCapacity , Weight)); 
				return false;
			}
			else
			{
				TotalWeight += (Datas->Weight*Datas->PickupAmmo);
				PointItem->ID == "1" ? Ammo556 += Datas->PickupAmmo : Ammo762 += Datas->PickupAmmo;
				ItemsInBackpack.Add(PointItem);
			}
			break;
		}
	case EItemType::EIT_Health :
		{
			auto Datas = ItemHealthTable->FindRow<FItemHealth>(PointItem->ID,TEXT(""));
			if (TotalWeight+Datas->Weight > BackpackCapacity)
			{
				int32 Weight = TotalWeight+Datas->Weight;
				GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("가방 무게(%d) 초과 : %d"), BackpackCapacity , Weight)); 
				return false;
			}
			else
			{
				TotalWeight += Datas->Weight;
				ItemsInBackpack.Add(PointItem);
			}
			break;
		}
	case EItemType::EIT_Boost :
		{
			auto Datas = ItemBoostTable->FindRow<FItemBoost>(PointItem->ID,TEXT(""));
			if (TotalWeight+Datas->Weight > BackpackCapacity)
			{
				int32 Weight = TotalWeight+Datas->Weight;
				GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("가방 무게(%d) 초과 : %d"), BackpackCapacity , Weight)); 
				return false;
			}
			else
			{
				TotalWeight += Datas->Weight;
				ItemsInBackpack.Add(PointItem);
			}
			break;
		}
	case EItemType::EIT_Helmet :
		{
			//auto Datas = ItemEquipmentsTable->FindRow<FItemEquipment>(PointItem->ID,TEXT(""));
			for (auto Array : ItemsInBackpack)
			{
				if(Array->ID == PointItem->ID && Array->ItemType == PointItem->ItemType)
				{
					GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("동일한 레벨 헷맷이 있음 ")));
					return false;
				}
			}
			
			ItemsInBackpack.Add(PointItem);
			break;
		}
	case EItemType::EIT_Vest :
		{
			//auto Datas = ItemEquipmentsTable->FindRow<FItemEquipment>(PointItem->ID,TEXT(""));
			for (auto Array : ItemsInBackpack)
			{
				if(Array->ID == PointItem->ID && Array->ItemType == PointItem->ItemType)
				{
					GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("동일한 레벨 조끼가 있음 ")));
					return false;
				}
			}
			
			ItemsInBackpack.Add(PointItem);
			break;
		}
	case EItemType::EIT_Backpack :
		{
			//auto Datas = ItemEquipmentsTable->FindRow<FItemEquipment>(PointItem->ID,TEXT(""));
			for (auto Array : ItemsInBackpack)
			{
				if(Array->ID == PointItem->ID && Array->ItemType == PointItem->ItemType)
				{
					GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("동일한 레벨 가방이 있음 ")));
					return false;
				}
			}
			
			ItemsInBackpack.Add(PointItem);
			break;
		}
	case EItemType::EIT_Clothes :
		{
			//auto Datas = ItemClothesTable->FindRow<FItemClothes>(PointItem->ID,TEXT(""));
			for (auto Array : ItemsInBackpack)
			{
				if(Array->ID == PointItem->ID && Array->ItemType == PointItem->ItemType)
				{
					GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("동일한 옷이 있음 ")));
					return false;
				}
			}
			ItemsInBackpack.Add(PointItem);
			break;
		}
	}

	// GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("%d 아이템 개수"), ItemsInBackpack.Num())); 
	// for (auto Array : ItemsInBackpack)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("%s 의 가방내 아이템 추가 : %s"), *GetOwner()->GetName(), *Array->GetName())); 
	// }
	// GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("-----------------------------------------")));

	return true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UInventoryComponent::AddPickupItems(AItemPickupBase* Items)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddPickupItems(Items);
	}
	else if (GetOwnerRole() == ROLE_Authority)
	{
		ItemsInRange.Add(Items);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,FString::Printf(TEXT("%s의 주위에 %s 아이템이 있습니다."), *GetOwner()->GetName(), *Items->GetName()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,FString::Printf(TEXT("ItemsInRange 개수 : %d ."), ItemsInRange.Num()));
		if (AddPickupItemList.IsBound())
		{
			AddPickupItemList.Broadcast();
		}
	}
}

void UInventoryComponent::SubPickupItems(AItemPickupBase* Items)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerSubPickupItems(Items);
	}
	else if (GetOwnerRole() == ROLE_Authority)
	{
		if (ItemsInRange.Find(Items) != -1)
		{
			ItemsInRange.RemoveSingle(Items);
		}

		if (AddPickupItemList.IsBound())
		{
			SubPickupItemList.Broadcast();
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,FString::Printf(TEXT("%s이 %s 아이템 주위를 벗어 났습니다.."), *GetOwner()->GetName(), *Items->GetName()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,FString::Printf(TEXT("ItemsInRange 개수 : %d ."), ItemsInRange.Num()));
}

void UInventoryComponent::DiscardWeapon(AEquipmentsWeapon* ItemWeapon)
{
	APortfolioPlayerController* PC = Cast<APortfolioPlayerController>(GetOwner());
	AItemPickupBase* PickupItem;
	
	if (ItemWeapon->Ammo > 0)
	{
		FName ID = ItemWeapon->Datas->UseAmmoID;
		int32 Randomint = rand();
		FString SN = FString::FromInt(Randomint);
		FTransform Transform = UKismetMathLibrary::MakeTransform({}, {}, FVector(1.0f, 1.0f, 1.0f));
		AEquipmentsAmmo* SpawnObject = GetWorld()->SpawnActorDeferred<AEquipmentsAmmo>(AEquipmentsAmmo::StaticClass(), Transform);
		SpawnObject->ID = ID;
		SpawnObject->SN = SN;
		SpawnObject->Amount = ItemWeapon->Ammo;
		SpawnObject->FinishSpawning(Transform);
		PC->SpawnPickupItem(SpawnObject, PickupItem);
		ItemWeapon->Ammo = 0;
	}

	PC->SpawnPickupItem(ItemWeapon, PickupItem);

	if (IsValid(ItemWeapon->AccSightObj))
	{
		PC->SpawnPickupItem(ItemWeapon->AccSightObj, PickupItem);
		ItemWeapon->AccSightObj->Destroy();
	}
}


void UInventoryComponent::DiscardItem(AItemBase* Item)
{
	APortfolioPlayerController* PC = Cast<APortfolioPlayerController>(GetOwner());
	AItemPickupBase* PickupItem;
	PC->SpawnPickupItem(Item, PickupItem);
	if (Item->ItemType == EItemType::EIT_Ammo)
	{
		//updateammoamount
		
		RemoveItem(Item);
		if (IsValid(Item))
		{
			Item->Destroy();
		}
	}
}

bool UInventoryComponent::RemoveItem(AItemBase* ItemRef)
{
	const auto Item = Cast<AItemPickupBase>(ItemRef);
	if (ItemsInBackpack.Find(Item) != -1)
	{
		ItemsInBackpack.RemoveSingle(Item);
		return true;
	}
	else if (ItemsInBackpack.Find(Item) == -1)
	{
		return false;
	}
	return false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryComponent, ItemsInRange);
	DOREPLIFETIME(UInventoryComponent, ItemsInBackpack);
	DOREPLIFETIME(UInventoryComponent, BackpackCapacity);
	DOREPLIFETIME(UInventoryComponent, TotalWeight);
	DOREPLIFETIME(UInventoryComponent, Ammo556);
	DOREPLIFETIME(UInventoryComponent, Ammo762);
}


void UInventoryComponent::ServerSubPickupItems_Implementation(AItemPickupBase* Items)
{
	SubPickupItems(Items);
}

bool UInventoryComponent::ServerSubPickupItems_Validate(AItemPickupBase* Items)
{
	return true;
}

void UInventoryComponent::ServerAddPickupItems_Implementation(AItemPickupBase* Items)
{
	AddPickupItems(Items);
}

bool UInventoryComponent::ServerAddPickupItems_Validate(AItemPickupBase* Items)
{
	return true;
}