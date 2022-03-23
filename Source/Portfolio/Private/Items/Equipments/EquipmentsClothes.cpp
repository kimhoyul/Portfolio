// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipments/EquipmentsClothes.h"
#include "Engine/DataTable.h"

AEquipmentsClothes::AEquipmentsClothes()
{
	const FString ItemClothesDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Clothes.DT_Item_Clothes");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Clothes(*ItemClothesDataPath);
	check(DT_Item_Clothes.Succeeded());
	ItemClothesTable = DT_Item_Clothes.Object;
	check(ItemClothesTable->GetRowMap().Num() > 0);
}

void AEquipmentsClothes::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Datas = ItemClothesTable->FindRow<FItemClothes>(ID,TEXT(""));
	ClothesType = Datas->Type;
	Init(EItemType::EIT_Clothes, Datas->Name);
}
