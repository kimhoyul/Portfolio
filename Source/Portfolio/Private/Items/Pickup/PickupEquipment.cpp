// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup/PickupEquipment.h"

#include "Kismet/KismetSystemLibrary.h"

APickupEquipment::APickupEquipment()
{
	const FString ItemEquipmentsDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Equipment.DT_Item_Equipment");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Equipment(*ItemEquipmentsDataPath);
	check(DT_Item_Equipment.Succeeded());
	ItemEquipmentsTable = DT_Item_Equipment.Object;
	check(ItemEquipmentsTable->GetRowMap().Num() > 0);

	bReplicates = true;
}

void APickupEquipment::BeginPlay()
{
	Super::BeginPlay();

	FVector Origin;
	FVector StaticMeshBound;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(StaticMesh, Origin, StaticMeshBound, SphereRadius);
	
	if (EquipmentType == EItemType::EIT_Helmet)
	{
		AddActorLocalOffset(FVector(0.0f, 0.0f, StaticMeshBound.Z * 2.0f));
	}
	else
	{
		AddActorLocalOffset(FVector(0.0f, 0.0f, StaticMeshBound.Z));
	}

	if (ID != "None")
	{
		Datas = ItemEquipmentsTable->FindRow<FItemEquipment>(ID,TEXT(""));

		EquipmentType = Datas->Type;
		FText SWITCHTO = FText::FromString(FString("SWITCH TO ")); 
		InitPickup(EquipmentType, Datas->Name, SWITCHTO, Datas->StaticMesh);

		if (Datas->Type == EItemType::EIT_Vest)
		{
			StaticMesh->SetWorldScale3D(FVector(1.0f, 0.1f, 1.0f));
			StaticMesh->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f), false, nullptr, ETeleportType::TeleportPhysics);
		}

		if (Datas->Type == EItemType::EIT_Backpack)
		{
			StaticMesh->SetWorldScale3D(FVector(1.0f, 0.1f, 1.0f));
			StaticMesh->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f), false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}
