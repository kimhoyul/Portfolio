// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Use/UseItemBoost.h"

AUseItemBoost::AUseItemBoost()
{
	const FString ItemBoostDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Boost.DT_Item_Boost");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Boost(*ItemBoostDataPath);
	check(DT_Item_Boost.Succeeded());
	ItemBoostTable = DT_Item_Boost.Object;
	check(ItemBoostTable->GetRowMap().Num() > 0);
}

void AUseItemBoost::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Datas = ItemBoostTable->FindRow<FItemBoost>(ID,TEXT(""));
	Init(EItemType::EIT_Boost, Datas->Name);
	UseTime = Datas->UseTime;
}

void AUseItemBoost::GetWeight(int32& Weight)
{
	Weight = Amount * Datas->Weight;
}