// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "PickupWeapon.generated.h"

UCLASS()
class PORTFOLIO_API APickupWeapon : public AItemPickupBase
{
	GENERATED_BODY()

	APickupWeapon();
	
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	int32 Ammo;
	
	FItemWeapon* Datas;
	
	UPROPERTY(BlueprintReadOnly)
	FItemWeapon DatasRef;

private:
	UPROPERTY()
	UDataTable* ItemWeaponTable;
};
