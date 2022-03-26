// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup/PickupClothes.h"

#include "Kismet/KismetSystemLibrary.h"

APickupClothes::APickupClothes()
{
	const FString ItemClothesDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Clothes.DT_Item_Clothes");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Clothes(*ItemClothesDataPath);
	check(DT_Item_Clothes.Succeeded());
	ItemClothesTable = DT_Item_Clothes.Object;
	check(ItemClothesTable->GetRowMap().Num() > 0);
}

void APickupClothes::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}
	
	FVector Origin;
	FVector StaticMeshBound;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(GetIStaticMeshComponent(), Origin, StaticMeshBound, SphereRadius);
	
	AddActorLocalOffset(FVector(0.0f, 0.0f, StaticMeshBound.Z));

	if (ID != "None")
	{
		Datas = ItemClothesTable->FindRow<FItemClothes>(ID,TEXT(""));
		ClothesType = Datas->Type;
		FText EQUIP = FText::FromString(FString("EQUIP ")); 
		InitPickup(EItemType::EIT_Clothes, Datas->Name, EQUIP, Datas->StaticMesh);

		DatasRef = *Datas;
		
		if (ClothesType != EClothesType::ECT_Shoes)
		{		
			GetIStaticMeshComponent()->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
			GetIStaticMeshComponent()->SetWorldScale3D(FVector(1.0f, 0.1f, 1.0f));
		}
	}
	
}
