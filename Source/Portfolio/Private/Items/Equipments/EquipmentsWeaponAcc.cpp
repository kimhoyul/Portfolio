// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipments/EquipmentsWeaponAcc.h"

AEquipmentsWeaponAcc::AEquipmentsWeaponAcc()
{
	const FString ItemWeaponAccDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_WeaponAcc.DT_Item_WeaponAcc");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_WeaponAcc(*ItemWeaponAccDataPath);
	check(DT_Item_WeaponAcc.Succeeded());
	ItemWeaponAccTable = DT_Item_WeaponAcc.Object;
	check(ItemWeaponAccTable->GetRowMap().Num() > 0);
}

void AEquipmentsWeaponAcc::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Datas = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(ID,TEXT(""));
	Init(EItemType::EIT_Accesories, Datas->Name);

	AccType = Datas->Type;
}

void AEquipmentsWeaponAcc::GetWeight(int32& Weight)
{
	Weight = Datas->Weight;
}

