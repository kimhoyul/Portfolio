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
	
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void GetWeight(int32& Weight) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	EWeaponAccType AccType;

	FItemWeaponAcc* Datas;

private:
	UPROPERTY()
	UDataTable* ItemWeaponAccTable;
};
