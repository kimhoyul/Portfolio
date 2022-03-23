// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipments/EquipmentsAmmo.h"

AEquipmentsAmmo::AEquipmentsAmmo()
{
	const FString ItemAmmoDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Ammo.DT_Item_Ammo");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Ammo(*ItemAmmoDataPath);
	check(DT_Item_Ammo.Succeeded());
	ItemAmmoTable = DT_Item_Ammo.Object;
	check(ItemAmmoTable->GetRowMap().Num() > 0);
}

void AEquipmentsAmmo::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Datas = ItemAmmoTable->FindRow<FItemAmmo>(ID,TEXT(""));
	Init(EItemType::EIT_Ammo, Datas->Name);
}

void AEquipmentsAmmo::GetWeight(int32& Weight)
{
	Weight = Amount * Datas->Weight;
}



