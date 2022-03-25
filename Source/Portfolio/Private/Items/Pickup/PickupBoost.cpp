// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup/PickupBoost.h"

APickupBoost::APickupBoost()
{
	const FString ItemBoostDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Boost.DT_Item_Boost");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Boost(*ItemBoostDataPath);
	check(DT_Item_Boost.Succeeded());
	ItemBoostTable = DT_Item_Boost.Object;
	check(ItemBoostTable->GetRowMap().Num() > 0);
}

void APickupBoost::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}
	
	Datas = ItemBoostTable->FindRow<FItemBoost>(ID,TEXT(""));
	if (Datas)
	{
		SingleWeight = Datas->Weight;
		FText PICKUP = FText::FromString(FString("PICK UP ")); 
		InitPickup(EItemType::EIT_Boost, Datas->Name, PICKUP, Datas->StaticMesh);
	}

	DatasRef = *Datas;
}

void APickupBoost::GetWeight(int32& Weight)
{
	Weight = Amount * SingleWeight;
}
