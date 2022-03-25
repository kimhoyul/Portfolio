// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup/PickupHealth.h"
#include "Player/PortfolioCharacter.h"
#include "Player/Components/PlayerStatComponent.h"

APickupHealth::APickupHealth()
{
	const FString ItemHealthDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Health.DT_Item_Health");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Health(*ItemHealthDataPath);
	check(DT_Item_Health.Succeeded());
	ItemHealthTable = DT_Item_Health.Object;
	check(ItemHealthTable->GetRowMap().Num() > 0);
}

void APickupHealth::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}
	
	if(ID != "None")
	{
		Datas = ItemHealthTable->FindRow<FItemHealth>(ID,TEXT(""));
		SingleWeight = Datas->Weight;
		FText PICKUP = FText::FromString(FString("PICK UP ")); 
		InitPickup(EItemType::EIT_Health, Datas->Name, PICKUP, Datas->StaticMesh);
	}
	
	DatasRef = *Datas;
}

void APickupHealth::GetWeight(int32& Weight)
{
	Weight = Amount * SingleWeight;
}

void APickupHealth::UseItem(APortfolioCharacter* Player, FName Item)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (Item == "1")
		{
			Player->PlayerStatComponent->AddHealth(Datas->Value);
		}
		else if (Item == "2")
		{
			Player->PlayerStatComponent->AddHealth(Datas->Value);
		}
		else if (Item == "3")
		{
			Player->PlayerStatComponent->AddHealth(Datas->Value);
		}
		else if (Item == "4")
		{
			Player->PlayerStatComponent->AddHunger(Datas->Value);
		}
		else if (Item == "5")
		{
			Player->PlayerStatComponent->AddThirst(Datas->Value);
		}

		Destroy();
	}
}
