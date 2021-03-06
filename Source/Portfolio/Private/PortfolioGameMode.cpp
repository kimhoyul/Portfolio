// Copyright Epic Games, Inc. All Rights Reserved.


//////////////////////////////////////////////////////////////////////////
// Base
#include "PortfolioGameMode.h"

//////////////////////////////////////////////////////////////////////////
// Engine Function
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// GameFrameWork Ref
#include "PortfolioGameInstance.h"

//////////////////////////////////////////////////////////////////////////
// Class Setting
#include "Player/PortfolioCharacter.h"
#include "UI/GameHUD.h"
#include "Player/PortfolioPlayerController.h"
#include "Player/PortfolioPlayerState.h"
#include "PortfolioGameStateBase.h"

//////////////////////////////////////////////////////////////////////////
// Item Generation Ref
#include "Items/ItemGroup.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "Items/Pickup/PickupAmmo.h"
#include "Items/Pickup/PickupBoost.h"
#include "Items/Pickup/PickupClothes.h"
#include "Items/Pickup/PickupEquipment.h"
#include "Items/Pickup/PickupHealth.h"
#include "Items/Pickup/PickupWeapon.h"
#include "Items/Pickup/PickupWeaponAcc.h"

//////////////////////////////////////////////////////////////////////////
// Spawn Ref
#include "Spawn/SpawnPoint.h"

DEFINE_LOG_CATEGORY_STATIC(LogPortfolioGameMode, All, All);

APortfolioGameMode::APortfolioGameMode()
{
	//////////////////////////////////////////////////////////////////////////
	// Class Setting
	DefaultPawnClass = APortfolioCharacter::StaticClass();
	PlayerControllerClass = APortfolioPlayerController::StaticClass();
	HUDClass = AGameHUD::StaticClass();
	PlayerStateClass = APortfolioPlayerState::StaticClass();
	GameStateClass = APortfolioGameStateBase::StaticClass();

	//////////////////////////////////////////////////////////////////////////
	// Get DT_SpawnLocation
	const FString SpawnPointLocationDataPath = TEXT("/Game/Datas/DataTables/Respawn/DT_SpawnPoint.DT_SpawnPoint");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SpawnPointLocation(*SpawnPointLocationDataPath);
	check(DT_SpawnPointLocation.Succeeded());
	SpawnPointLocationTable = DT_SpawnPointLocation.Object;
	check(SpawnPointLocationTable->GetRowMap().Num() > 0);
	
	//////////////////////////////////////////////////////////////////////////
	// Get DT_ItemGroupLocation
	const FString ItemGroupLocationDataPath = TEXT("/Game/Datas/DataTables/ItemGeneration/DT_ItemGroupLocation.DT_ItemGroupLocation");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemGroupLocation(*ItemGroupLocationDataPath);
	check(DT_ItemGroupLocation.Succeeded());
	ItemGroupLocationTable = DT_ItemGroupLocation.Object;
	check(ItemGroupLocationTable->GetRowMap().Num() > 0);

	//////////////////////////////////////////////////////////////////////////
	// Get DT_Numberprobability
	const FString NumberProbabilityDataPath = TEXT("/Game/Datas/DataTables/ItemGeneration/DT_Numberprobability.DT_Numberprobability");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Numberprobability(*NumberProbabilityDataPath);
	check(DT_Numberprobability.Succeeded());
	NumberProbabilityTable = DT_Numberprobability.Object;
	check(NumberProbabilityTable->GetRowMap().Num() > 0);

	//////////////////////////////////////////////////////////////////////////
	// Get DT_ItemTypeprobability
	const FString ItemTypeProbabilityDataPath = TEXT("/Game/Datas/DataTables/ItemGeneration/DT_ItemTypeprobability.DT_ItemTypeprobability");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemTypeprobability(*ItemTypeProbabilityDataPath);
	check(DT_ItemTypeprobability.Succeeded());
	ItemTypeProbabilityTable = DT_ItemTypeprobability.Object;
	check(ItemTypeProbabilityTable->GetRowMap().Num() > 0);

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
	const FString ItemHealthDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Health1.DT_Item_Health1");
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
}

void APortfolioGameMode::BeginPlay()
{
	//////////////////////////////////////////////////////////////////////////
	// Cast PortfolioGameInstance Ref
	GameInstanceRef = Cast<UPortfolioGameInstance>(GetGameInstance());

	//////////////////////////////////////////////////////////////////////////
	// Cast PortfolioPlayerController Ref 
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		Controller = Cast<APortfolioPlayerController>(*It);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Generate Random Items 
	TArray<AItemPickupBase*> Items;
	GenerateItems(Items);

	//////////////////////////////////////////////////////////////////////////
	// SetPickupItems
	Controller->SetPickupItems(Items);

	//////////////////////////////////////////////////////////////////////////
	// Set Random & Default Spawn Points
	TArray<FName> RowNames = SpawnPointLocationTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FSpawnPoints* SpawnPointsData = SpawnPointLocationTable->FindRow<FSpawnPoints>(RowName,TEXT(""));
		SpawnPoints.Add(SpawnPointsData->SpawnLocation);
	}
	DefaultSpawnLocation = FVector(-58198.562500,-159507.109375,-6536.640625);
	RespawnTime = 3.0f;
}

//////////////////////////////////////////////////////////////////////////
// Spawn
void APortfolioGameMode::Spawn(APortfolioPlayerController* ControllerRef)
{
	FVector Location = GetSpawnPoint();
	FRotator Rotation = FRotator::ZeroRotator;
	if (APawn* Pawn =  GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, Rotation))
	{
		ControllerRef->Possess(Pawn);
	}
}

FVector APortfolioGameMode::GetSpawnPoint()
{
	for (int32 i = 0; i < SpawnPoints.Num(); ++i)
	{
		int32 Slot = FMath::RandRange(0, SpawnPoints.Num() -1);
		if (SpawnPoints[Slot] != FVector::ZeroVector)
		{
			return SpawnPoints[Slot];
		}
	}
	return DefaultSpawnLocation;
}

void APortfolioGameMode::Respawn(APortfolioPlayerController* ControllerRef)
{
	if (ControllerRef)
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			FTimerDelegate RespawnDelegate;
			RespawnDelegate.BindUFunction(this, FName("Spawn"), ControllerRef);
			FTimerHandle RespawnHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnHandle, RespawnDelegate, 3.0f, false);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// GenerateItems
void APortfolioGameMode::GenerateItems(TArray<AItemPickupBase*>& Items)
{
	TArray<FTransform> UsedLocation;
	FTransform SpawnLocation;
	TArray<AItemPickupBase*> ItemsObject;
	AItemGroup* ItemGroupRef = nullptr;

	//////////////////////////////////////////////////////////////////////////
	// Get all ItemGroup Location
	TArray<FName> RowNames = ItemGroupLocationTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FItemGroupLocation* ItemGroupDatas = ItemGroupLocationTable->FindRow<FItemGroupLocation>(RowName,TEXT(""));

		//////////////////////////////////////////////////////////////////////////
		// Spawn a ItemGroup Actor
		FActorSpawnParameters SpawnParameter;
		FTransform ActorSpawnLocation = FTransform(ItemGroupDatas->Location);
		SpawnParameter.Owner = this;
		ItemGroupRef = GetWorld()->SpawnActor<
			AItemGroup>(AItemGroup::StaticClass(), ActorSpawnLocation, SpawnParameter);

		//////////////////////////////////////////////////////////////////////////
		// Number of random items
		int32 Number;
		RandomItemNumber(Number);

		//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("RandomItemNumber : [%d]"), Number));

		//////////////////////////////////////////////////////////////////////////
		// Random Items type
		EItemType ItemType;
		TArray<EItemType> ItemTypeArr;
		TArray<FItemTypeAndID> ItemsIDArr;
		FName SpawnID;

		for (int i = 0; i < Number; ++i)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("RandomItemType ?????? : [%d] ??????"), i + 1));
			RandomItemType(ItemType);

			//FString Type = GetEItemTypeAsString(ItemType);
			if (ItemTypeArr.Find(ItemType) == -1)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("ItemTypeArr ??? ????????? ItemType = %s"), *Type));
				//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("---------------------------------------------------------------")));
				ItemTypeArr.Add(ItemType);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("ItemTypeArr ??? ?????? ?????? ????????? ItemType = %s"), *Type));
				--i;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Random Items ID
		for (EItemType RandItemType : ItemTypeArr)
		{
			FItemTypeAndID Item;
			RandomItemID(RandItemType, Item);

			//FString Type = GetEItemTypeAsString(Item.Type);
			//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("RandomItemID Type = %s, ID = %s"), *Type, *FText::FromName(Item.ID).ToString()));

			ItemsIDArr.Add(Item);
		}

		ItemTypeArr.Empty();
		//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("---------------------------------------------------------------")));

		//////////////////////////////////////////////////////////////////////////
		// Find weapon item
		int32 Findweapon = 0;
		for (FItemTypeAndID ItemTypeAndID : ItemsIDArr)
		{
			Findweapon++;
			//FString Type = GetEItemTypeAsString(ItemTypeAndID.Type);
			//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("??????????????? ?????? [%d]?????? Type = %s"), Findweapon, *Type));

			if (ItemTypeAndID.Type == EItemType::EIT_Weapon)
			{
				//////////////////////////////////////////////////////////////////////////
				// Creat a Ammo
				auto Datas = ItemWeaponTable->FindRow<FItemWeapon>(ItemTypeAndID.ID,TEXT(""));
				FItemTypeAndID AddRef;
				AddRef.Type = EItemType::EIT_Ammo;
				AddRef.ID = Datas->UseAmmoID;
				//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("????????? ?????? = [%d], ????????? UseAmmoID = %s"), Findweapon, *FText::FromName(AddRef.ID).ToString()));
				ItemsIDArr.Add(AddRef);
				//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, FString::Printf(TEXT("?????? ????????? ??? ItemsIDArr ?????? = %d"), ItemsIDArr.Num()));
				//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("---------------------------------------------------------------")));
				
				//////////////////////////////////////////////////////////////////////////
				// Remove a Item
				int32 Index = 0;
				for (FItemTypeAndID Array : ItemsIDArr)
				{
					if (Array.Type != EItemType::EIT_Weapon)
					{
						//FString Type1 = GetEItemTypeAsString(Array.Type);
						//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("?????? ???????????? ItemsIDArr[%d] ??? Item Type = %s"), Index,*Type1));
						ItemsIDArr.RemoveAt(Index);
						break;
					}
					Index++;
				}
				//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("????????? ????????? ??? ItemsIDArr ?????? = %d"), ItemsIDArr.Num()));
				break;
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("---------------------------------------------------------------")));

		//////////////////////////////////////////////////////////////////////////
		// Loop all items ready to be generated
		EItemType SpawnType;

		int testnum = 0;
		for (FItemTypeAndID Array : ItemsIDArr)
		{
			testnum++;
			SpawnType = Array.Type;
			SpawnID = Array.ID;
			//FString Type = GetEItemTypeAsString(SpawnType);
			//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("%d?????? ????????? ????????? Type = %s, ID = %s"), testnum, *Type, *FText::FromName(SpawnID).ToString()));

			//////////////////////////////////////////////////////////////////////////
			// Random Location
			TArray<FTransform> AllTransform;

			ItemGroupRef->GetAllTransform(AllTransform);

			for (int i = 0; i < 1; ++i)
			{
				RandomLocation(AllTransform, SpawnLocation);
				bool A = true;
				for (int j = 0; j < UsedLocation.Num(); j++)
				{
					A = A && !UKismetMathLibrary::EqualEqual_TransformTransform(UsedLocation[j], SpawnLocation);
				}
				if (A == true)
				{
					UsedLocation.Add(SpawnLocation);
				}
				else
				{
					--i;
				}
			}
		

			//////////////////////////////////////////////////////////////////////////
			// Spawn Actor
			switch (SpawnType)
			{
			case EItemType::EIT_Weapon:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupWeapon>(APickupWeapon::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 1;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			case EItemType::EIT_Accesories:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupWeaponAcc>(APickupWeaponAcc::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 1;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			case EItemType::EIT_Ammo:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupAmmo>(APickupAmmo::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 0;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			case EItemType::EIT_Health:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupHealth>(APickupHealth::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 1;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			case EItemType::EIT_Boost:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupBoost>(APickupBoost::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 1;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			case EItemType::EIT_Helmet:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 1;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			case EItemType::EIT_Vest:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 1;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			case EItemType::EIT_Backpack:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 1;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			case EItemType::EIT_Clothes:
				{
					AItemPickupBase* SpawnObject = GetWorld()->SpawnActorDeferred<APickupClothes>(APickupClothes::StaticClass(), SpawnLocation);
					SpawnObject->ID = SpawnID;
					SpawnObject->SN = GameInstanceRef->GenerateSN();
					SpawnObject->Amount = 1;
					SpawnObject->FinishSpawning(SpawnLocation);
					ItemsObject.Add(SpawnObject);
					break;
				}
			}
		}
		ItemsIDArr.Empty();
	}

	Items = ItemsObject; 
}

void APortfolioGameMode::RandomItemNumber(int32 &Number)
{
	TArray<int32> IntArr;
	
	TArray<FName> RowNames = NumberProbabilityTable->GetRowNames();

	//////////////////////////////////////////////////////////////////////////
	// Number of random items
	for (FName RowName : RowNames)
	{
		NumberProbabilityData = NumberProbabilityTable->FindRow<FNumberProbability>(RowName,TEXT(""));

		
		for (int i = 1; i <= NumberProbabilityData->Percent; ++i)
		{
			IntArr.Add(NumberProbabilityData->Number);
		}
	}

	TArray<EItemType> FakeEnumArr;
	TArray<FItemTypeAndID> FakeStructArr;
	ShuffleArray(IntArr, FakeEnumArr, FakeStructArr);
	
	int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(0, 99);
	
	Number = IntArr[RandomInt];
}

void APortfolioGameMode::RandomItemType(EItemType& ItemType)
{
	TArray<EItemType> EnumArr;
	
	TArray<FName> RowNames = ItemTypeProbabilityTable->GetRowNames();

	//////////////////////////////////////////////////////////////////////////
	// Number of random items
	for (FName RowName : RowNames)
	{
		ItemTypeProbabilityData = ItemTypeProbabilityTable->FindRow<FItemTypeProbability>(RowName,TEXT(""));

		
		for (int i = 1; i <= ItemTypeProbabilityData->Percent; ++i)
		{
			EnumArr.Add(ItemTypeProbabilityData->Type);
		}
	}

	TArray<int32> FakeIntArr;
	TArray<FItemTypeAndID> FakeStructArr;
	ShuffleArray(FakeIntArr, EnumArr, FakeStructArr);
	
	int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(0, 99);
	ItemType = EnumArr[RandomInt];
}

void APortfolioGameMode::RandomItemID(EItemType ItemTypeRef, FItemTypeAndID& ItemRef)
{
	TArray<FItemTypeAndID> ItemTypeAndIdsArr;
	
	switch (ItemTypeRef)
	{
	case EItemType::EIT_Weapon :
		{
			TArray<FName> RowNames = ItemWeaponTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				ItemWeaponData = ItemWeaponTable->FindRow<FItemWeapon>(RowName,TEXT(""));
				
				for (int i = 1; i <= ItemWeaponData->ProbabilityPercent; ++i)
				{
					FItemTypeAndID AddRef;
					AddRef.Type = EItemType::EIT_Weapon;
					AddRef.ID = RowName;
					ItemTypeAndIdsArr.Add(AddRef);
				}
			}
			break;
		}
	case EItemType::EIT_Accesories :
		{
			TArray<FName> RowNames = ItemWeaponAccTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				FItemTypeAndID AddRef;
				AddRef.Type = EItemType::EIT_Accesories;
				AddRef.ID = RowName;
				ItemTypeAndIdsArr.Add(AddRef);
			}
			break;
		}
	case EItemType::EIT_Ammo :
		{
			TArray<FName> RowNames = ItemAmmoTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				ItemAmmoData = ItemAmmoTable->FindRow<FItemAmmo>(RowName,TEXT(""));
				
				for (int i = 1; i <= ItemAmmoData->ProbabilityPercent; ++i)
				{
					FItemTypeAndID AddRef;
					AddRef.Type = EItemType::EIT_Ammo;
					AddRef.ID = RowName;
					ItemTypeAndIdsArr.Add(AddRef);
				}
			}
			break;
		}
	case EItemType::EIT_Health :
		{
			TArray<FName> RowNames = ItemHealthTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				ItemHealthData = ItemHealthTable->FindRow<FItemHealth>(RowName,TEXT(""));
				
				for (int i = 1; i <= ItemHealthData->ProbabilityPercent; ++i)
				{
					FItemTypeAndID AddRef;
					AddRef.Type = EItemType::EIT_Health;
					AddRef.ID = RowName;
					ItemTypeAndIdsArr.Add(AddRef);
				}
			}
			break;
		}
	case EItemType::EIT_Boost :
		{
			TArray<FName> RowNames = ItemBoostTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				ItemBoostData = ItemBoostTable->FindRow<FItemBoost>(RowName,TEXT(""));
				
				for (int i = 1; i <= ItemBoostData->ProbabilityPercent; ++i)
				{
					FItemTypeAndID AddRef;
					AddRef.Type = EItemType::EIT_Boost;
					AddRef.ID = RowName;
					ItemTypeAndIdsArr.Add(AddRef);
				}
			}
			break;
		}
	case EItemType::EIT_Helmet :
		{
			TArray<FName> RowNames = ItemEquipmentsTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				ItemEquipmentData = ItemEquipmentsTable->FindRow<FItemEquipment>(RowName,TEXT(""));

				if (ItemEquipmentData->Type == EItemType::EIT_Helmet)
				{
					for (int i = 1; i <= ItemEquipmentData->ProbabilityPercent; ++i)
					{
						FItemTypeAndID AddRef;
						AddRef.Type = EItemType::EIT_Helmet;
						AddRef.ID = RowName;
						ItemTypeAndIdsArr.Add(AddRef);
					}
				}
			}
			break;
		}
	case EItemType::EIT_Vest :
		{
			TArray<FName> RowNames = ItemEquipmentsTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				ItemEquipmentData = ItemEquipmentsTable->FindRow<FItemEquipment>(RowName,TEXT(""));

				if (ItemEquipmentData->Type == EItemType::EIT_Vest)
				{
					for (int i = 1; i <= ItemEquipmentData->ProbabilityPercent; ++i)
					{
						FItemTypeAndID AddRef;
						AddRef.Type = EItemType::EIT_Vest;
						AddRef.ID = RowName;
						ItemTypeAndIdsArr.Add(AddRef);
					}
				}
			}
			break;
		}
	case EItemType::EIT_Backpack :
		{
			TArray<FName> RowNames = ItemEquipmentsTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				ItemEquipmentData = ItemEquipmentsTable->FindRow<FItemEquipment>(RowName,TEXT(""));

				if (ItemEquipmentData->Type == EItemType::EIT_Backpack)
				{
					for (int i = 1; i <= ItemEquipmentData->ProbabilityPercent; ++i)
					{
						FItemTypeAndID AddRef;
						AddRef.Type = EItemType::EIT_Backpack;
						AddRef.ID = RowName;
						ItemTypeAndIdsArr.Add(AddRef);
					}
				}
			}
			break;
		}
	case EItemType::EIT_Clothes :
		{
			TArray<FName> RowNames = ItemClothesTable->GetRowNames();
			
			for (FName RowName : RowNames)
			{
				if (RowName != "1" && RowName != "4")
				{
					FItemTypeAndID AddRef;
					AddRef.Type = EItemType::EIT_Clothes;
					AddRef.ID = RowName;
					ItemTypeAndIdsArr.Add(AddRef);
				}
			}
			break;
		}
	}

	TArray<int32> FakeIntArr;
	TArray<EItemType> FakeEnumArr;
	
	ShuffleArray(FakeIntArr, FakeEnumArr,ItemTypeAndIdsArr);
	int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(0, ItemTypeAndIdsArr.Num() - 1);
	ItemRef = ItemTypeAndIdsArr[RandomInt];
}

void APortfolioGameMode::RandomLocation(TArray<FTransform> Available, FTransform& Location)
{
	int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(0, Available.Num() - 1);
	Location = Available[RandomInt];
}

void APortfolioGameMode::ShuffleArray(TArray<int32>& IntArr, TArray<EItemType>& EnumArr, TArray<FItemTypeAndID>& StrutRef)
{
	if (IntArr.Num() > 0)
	{
		int32 LastIndex = IntArr.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				IntArr.Swap(i, Index);
			}
		}
	}
	else if (EnumArr.Num() > 0)
	{
		int32 LastIndex = EnumArr.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				EnumArr.Swap(i, Index);
			}
		}
	}
	else if (StrutRef.Num() > 0)
	{
		int32 LastIndex = StrutRef.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				StrutRef.Swap(i, Index);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Enum To String
FString APortfolioGameMode::GetEItemTypeAsString(EItemType EnumValue)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemType"), true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameStringByValue((int64)EnumValue);
}
