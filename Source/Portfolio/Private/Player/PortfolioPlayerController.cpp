// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PortfolioPlayerController.h"
#include "Components/BoxComponent.h"
#include "PortfolioCharacter.h"
#include "PortfolioPlayerState.h"
#include "Items/Pickup/PickupAmmo.h"
#include "Items/Pickup/PickupBoost.h"
#include "Items/Pickup/PickupClothes.h"
#include "Items/Pickup/PickupEquipment.h"
#include "Items/Pickup/PickupHealth.h"
#include "Items/Pickup/PickupWeapon.h"
#include "Items/Pickup/PickupWeaponAcc.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogPortfolioPlayerController, All, All);

void APortfolioPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Inventory", IE_Pressed, this, &APortfolioPlayerController::OnToggleInInventory);
}

void APortfolioPlayerController::OnToggleInInventory()
{
	GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("인입")));
}

void APortfolioPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PortfolioPlayerStateRef = Cast<APortfolioPlayerState>(PlayerState);
	if(!PortfolioPlayerStateRef)
	{
		GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("PortfolioPlayerController : MyPlayerState 생성 실패")));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("PortfolioPlayerController : MyPlayerState생성 : [%s]"), *PortfolioPlayerStateRef->GetName()));
	}
}

void APortfolioPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryActorTick.bCanEverTick = true;
	
}

bool APortfolioPlayerController::IsGameInputAllowed() const
{
	return !bCinematicMode;
}

//////////////////////////////////////////////////////////////////////////
// bind delegate
void APortfolioPlayerController::SetPickupItems(TArray<AItemPickupBase*> Items)
{
	PickupItems = Items;
	for (AItemPickupBase* ArrayElement : PickupItems)
	{
		ArrayElement->FOnOverlepBox.BindUFunction(this, FName("ExecBeginOverlap"));
		ArrayElement->FEndOverlapBox.BindUFunction(this, FName("ExecEndOverlap"));
	}
}

//////////////////////////////////////////////////////////////////////////
// In range Item Add
void APortfolioPlayerController::ExecBeginOverlap(AItemPickupBase* PickupObject, AActor* OtherActor)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerOnTest(PickupObject);
	}
	else
	{
		ItemsInRange.Add(PickupObject);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%d"), ItemsInRange.Num()));
}

//////////////////////////////////////////////////////////////////////////
// out of range item Remove
void APortfolioPlayerController::ExecEndOverlap(AItemPickupBase* PickupObject)
{
	// PickupObject->EnabledOutLine(false);
	
	if (ItemsInRange.Find(PickupObject) != -1)
	{
		ItemsInRange.RemoveSingle(PickupObject);
	}
	if (GetLocalRole() < ROLE_Authority)
	{
		
	}
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("out of range item : %s"), *PickupObject->GetName()));
}

void APortfolioPlayerController::OutlineIte(AItemPickupBase* Item)
{
	for (AItemPickupBase* ArrayElement : ItemsInRange)
	{
		if (ArrayElement == Item)
		{
			// Item->EnabledOutLine(true);
		}
		else
		{
			// Item->EnabledOutLine(false);
		}
	}
}

void APortfolioPlayerController::AutoPosition(EWeaponPosition& Position, bool& IsOnHand)
{
	//////////////////////////////////////////////////////////////////////////
	// Checck the Current Number of Weapons
	int32 CurrentAmount = 0;
	if (IsValid(PortfolioPlayerStateRef->GetWeapon1()))
	{
		CurrentAmount ++;
	}

	if (IsValid(PortfolioPlayerStateRef->GetWeapon2()))
	{
		CurrentAmount ++;
	}

	if (IsValid(PortfolioPlayerStateRef->GetHoldGun()))
	{
		CurrentAmount ++;
	}

	//////////////////////////////////////////////////////////////////////////
	// No weapon, place weapon in hand, belongs to left back
	if (CurrentAmount == 0)
	{
		Position = EWeaponPosition::EWP_Left;
		IsOnHand = true;
	}
	else
	{
		if (CurrentAmount < 2)
		{
			//////////////////////////////////////////////////////////////////////////
			// With fewer than two current weapon, return to the available back position
			if (IsValid(PortfolioPlayerStateRef->GetHoldGun()))
			{
				const EWeaponPosition WeaponPosition = PortfolioPlayerStateRef->GetHoldGun()->Position;
				switch (WeaponPosition)
				{
				case EWeaponPosition::EWP_Left :
					{
						Position = EWeaponPosition::EWP_Right;
						IsOnHand = false;
					}
				case  EWeaponPosition::EWP_Right :
					{
						Position = EWeaponPosition::EWP_Left;
						IsOnHand = false;
					}
				}
			}
			else
			{
				if (IsValid(PortfolioPlayerStateRef->GetWeapon1()))
				{
					Position = EWeaponPosition::EWP_Right;
					IsOnHand = false;
				}
				else
				{
					Position = EWeaponPosition::EWP_Left;
					IsOnHand = false;
				}
			}
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// No space available, priority is given to replacing hand-held weapon. if no hand-held weapon, left side of the back is replaced
			if (IsValid(PortfolioPlayerStateRef->GetHoldGun()))
			{
				const EWeaponPosition WeaponPosition = PortfolioPlayerStateRef->GetHoldGun()->Position;
				
				switch (WeaponPosition)
				{
				case EWeaponPosition::EWP_Left :
					{
						Position = EWeaponPosition::EWP_Left;
						IsOnHand = true;
					}
				case  EWeaponPosition::EWP_Right :
					{
						Position = EWeaponPosition::EWP_Right;
						IsOnHand = true;
					}
				}
			}
			else
			{
				Position = EWeaponPosition::EWP_Left;
				IsOnHand = false;
			}
		}
	}
}

void APortfolioPlayerController::AssignPosition(EWeaponPosition Assign, EWeaponPosition& Position, bool& IsOnHand)
{
	if (IsValid(PortfolioPlayerStateRef->GetHoldGun()))
	{
		if (PortfolioPlayerStateRef->GetHoldGun()->Position == Assign)
		{
			Position = Assign;
			IsOnHand = true;
		}
		else
		{
			Position = Assign;
			IsOnHand = false;
		}
	}
	else
	{
		Position = Assign;
		IsOnHand = false;
	}
}

void APortfolioPlayerController::SpawnPickupItem(AItemBase* ItemBase, AItemPickupBase*& PickupItem)
{
	FName ID = ItemBase->ID;
	FString SN = ItemBase->SN;
	FTransform Transform = UKismetMathLibrary::MakeTransform(PortfolioCharacterRef->GetActorLocation(), {}, FVector(1.0f, 1.0f, 1.0f));
	AItemPickupBase* CreateItem= nullptr;

	//////////////////////////////////////////////////////////////////////////
	// Spawn the corresponding item object into the level
	switch (ItemBase->ItemType)
	{
	case EItemType::EIT_Weapon:
		{
			APickupWeapon* SpawnObject = GetWorld()->SpawnActorDeferred<APickupWeapon>(APickupWeapon::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = 1;
			SpawnObject->Ammo = Cast<AEquipmentsWeapon>(ItemBase)->Ammo;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	case EItemType::EIT_Accesories:
		{
			APickupWeaponAcc* SpawnObject = GetWorld()->SpawnActorDeferred<APickupWeaponAcc>(APickupWeaponAcc::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = 1;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	case EItemType::EIT_Ammo:
		{
			APickupAmmo* SpawnObject = GetWorld()->SpawnActorDeferred<APickupAmmo>(APickupAmmo::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = ItemBase->Amount;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	case EItemType::EIT_Health:
		{
			APickupHealth* SpawnObject = GetWorld()->SpawnActorDeferred<APickupHealth>(APickupHealth::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = ItemBase->Amount;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	case EItemType::EIT_Boost:
		{
			APickupBoost* SpawnObject = GetWorld()->SpawnActorDeferred<APickupBoost>(APickupBoost::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = ItemBase->Amount;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	case EItemType::EIT_Helmet:
		{
			APickupEquipment* SpawnObject = GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = 1;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	case EItemType::EIT_Vest:
		{
			APickupEquipment* SpawnObject = GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = 1;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	case EItemType::EIT_Backpack:
		{
			APickupEquipment* SpawnObject = GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = 1;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	case EItemType::EIT_Clothes:
		{
			APickupClothes* SpawnObject = GetWorld()->SpawnActorDeferred<APickupClothes>(APickupClothes::StaticClass(), Transform);
			SpawnObject->ID = ID;
			SpawnObject->SN = SN;
			SpawnObject->Amount = 1;
			SpawnObject->FinishSpawning(Transform);
			CreateItem = SpawnObject;
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// the object is added to the Pickup Items array, and reactive collision detection
	PickupItems.Add(CreateItem);
	CreateItem->FOnOverlepBox.BindUFunction(this, FName("ExecBeginOverlap"));
	CreateItem->FEndOverlapBox.BindUFunction(this, FName("ExecEndOverlap"));
	CreateItem->GetIBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CreateItem->GetIBoxComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	PickupItem = CreateItem;
}

void APortfolioPlayerController::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	//TargetingItem();

	
}



void APortfolioPlayerController::ServerOnTest_Implementation(AItemPickupBase* PickupObject)
{
	ExecBeginOverlap(PickupObject, {});
}

bool APortfolioPlayerController::ServerOnTest_Validate(AItemPickupBase* PickupObject)
{
	return true;
}

void APortfolioPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(APortfolioPlayerController, ItemsInRange, COND_OwnerOnly);
}