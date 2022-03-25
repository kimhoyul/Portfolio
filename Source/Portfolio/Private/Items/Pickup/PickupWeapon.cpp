// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup/PickupWeapon.h"

APickupWeapon::APickupWeapon()
{
	//////////////////////////////////////////////////////////////////////////
	// Get data table
	
	const FString ItemWeaponDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Weapon.DT_Item_Weapon");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Weapon(*ItemWeaponDataPath);
	check(DT_Item_Weapon.Succeeded());
	ItemWeaponTable = DT_Item_Weapon.Object;
	check(ItemWeaponTable->GetRowMap().Num() > 0);
}

void APickupWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}

	if(ID != "None")
	{
		Datas = ItemWeaponTable->FindRow<FItemWeapon>(ID,TEXT(""));
		FText EQUIP = FText::FromString(FString("EQUIP "));
		InitPickup(EItemType::EIT_Weapon, Datas->Name, EQUIP, Datas->StaticMesh);
	}
	DatasRef = *Datas;;
}

