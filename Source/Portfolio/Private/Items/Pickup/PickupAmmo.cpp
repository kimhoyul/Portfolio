// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup/PickupAmmo.h"
#include "PortfolioGameInstance.h"

APickupAmmo::APickupAmmo()
{
	const FString ItemAmmoDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Ammo.DT_Item_Ammo");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Ammo(*ItemAmmoDataPath);
	check(DT_Item_Ammo.Succeeded());
	ItemAmmoTable = DT_Item_Ammo.Object;
	check(ItemAmmoTable->GetRowMap().Num() > 0);

	bReplicates = true;
	bool a = true;
}

void APickupAmmo::BeginPlay()
{
	Super::BeginPlay();
	GameInstanceRef = Cast<UPortfolioGameInstance>(GetGameInstance());
	
	if (ID != "None")
	{
		Datas = ItemAmmoTable->FindRow<FItemAmmo>(ID,TEXT(""));

		if (Amount == 0)
		{
			Amount = Datas->PickupAmmo;
			SingleWeight = Datas->Weight;

			FText PICKUP = FText::FromString(FString("PICK UP ")); 
			InitPickup(EItemType::EIT_Ammo, Datas->Name, PICKUP, Datas->PickupMesh);
		}
		else
		{
			SingleWeight = Datas->Weight;

			FText PICKUP = FText::FromString(FString("PICK UP ")); 
			InitPickup(EItemType::EIT_Ammo, Datas->Name, PICKUP, Datas->PickupMesh);
		}
	}
}

void APickupAmmo::GetWeight(int32& Weight)
{
	Weight = Amount * SingleWeight; 
}

void APickupAmmo::ServerSpawn_Implementation(FName IDRef)
{
	
}

bool APickupAmmo::ServerSpawn_Validate(FName IDRef)
{
	return true;
}
