// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipments/EquipmentsEquipment.h"

AEquipmentsEquipment::AEquipmentsEquipment()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	const FString ItemEquipmentsDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Equipment.DT_Item_Equipment");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Equipment(*ItemEquipmentsDataPath);
	check(DT_Item_Equipment.Succeeded());
	ItemEquipmentsTable = DT_Item_Equipment.Object;
	check(ItemEquipmentsTable->GetRowMap().Num() > 0);
}

void AEquipmentsEquipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Datas = ItemEquipmentsTable->FindRow<FItemEquipment>(ID,TEXT(""));
	Init(Datas->Type, Datas->Name);
	StaticMesh->SetStaticMesh(Datas->StaticMesh);
}

