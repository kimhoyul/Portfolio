// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup/PickupWeaponAcc.h"
#include "Kismet/KismetSystemLibrary.h"


APickupWeaponAcc::APickupWeaponAcc()
{
	const FString ItemWeaponAccDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_WeaponAcc.DT_Item_WeaponAcc");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_WeaponAcc(*ItemWeaponAccDataPath);
	check(DT_Item_WeaponAcc.Succeeded());
	ItemWeaponAccTable = DT_Item_WeaponAcc.Object;
	check(ItemWeaponAccTable->GetRowMap().Num() > 0);
}

void APickupWeaponAcc::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}
	
	if (AccType != EWeaponAccType::EWAT_Sight)
	{
		FVector Origin;
		FVector StaticMeshBound;
		float SphereRadius;
		UKismetSystemLibrary::GetComponentBounds(GetIStaticMeshComponent(), Origin, StaticMeshBound, SphereRadius);
		
		AddActorLocalOffset(FVector(0.0f, 0.0f, StaticMeshBound.Z));
	}

	if(ID != "None")
	{
		Datas = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(ID,TEXT(""));
		AccType = Datas->Type;
		FText PICKUP = FText::FromString(FString("PICK UP ")); 
		InitPickup(EItemType::EIT_Accesories, Datas->Name, PICKUP, Datas->StaticMesh);

		DatasRef = *Datas;
		
		switch (AccType)
		{
		case  EWeaponAccType::EWAT_Sight :
			{
				break;
			}
		case  EWeaponAccType::EWAT_Muzzle :
			{
				break;
			}
		case  EWeaponAccType::EWAT_Foregrip :
			{
				GetIStaticMeshComponent()->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
				break;
			}
		case  EWeaponAccType::EWAT_Mag :
			{
				GetIStaticMeshComponent()->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
				break;
			}
		case EWeaponAccType::EWAT_Buttstock :
			{
				GetIStaticMeshComponent()->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
				break;
			}
		}
	}
	
}

void APickupWeaponAcc::GetWeight(int32& Weight)
{
	Weight = Datas->Weight;
}
