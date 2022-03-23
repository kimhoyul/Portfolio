// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Use/UseItemHealth.h"

AUseItemHealth::AUseItemHealth()
{
	const FString ItemHealthDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Health.DT_Item_Health");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Health(*ItemHealthDataPath);
	check(DT_Item_Health.Succeeded());
	ItemHealthTable = DT_Item_Health.Object;
	check(ItemHealthTable->GetRowMap().Num() > 0);
}

void AUseItemHealth::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Datas = ItemHealthTable->FindRow<FItemHealth>(ID,TEXT(""));
	Init(EItemType::EIT_Health, Datas->Name);
	UseTime = Datas->UseTime;
}

void AUseItemHealth::GetWeight(int32& Weight)
{
	Weight = Amount * Datas->Weight;
}