// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "PickupWeaponAcc.generated.h"

UCLASS()
class PORTFOLIO_API APickupWeaponAcc : public AItemPickupBase
{
	GENERATED_BODY()

	APickupWeaponAcc();
	
	virtual void BeginPlay() override;

	virtual void GetWeight(int32& Weight) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	EWeaponAccType AccType;

	FItemWeaponAcc* Datas;

	UPROPERTY(BlueprintReadOnly)
	FItemWeaponAcc DatasRef;

private:
	UPROPERTY()
	UDataTable* ItemWeaponAccTable;
};
