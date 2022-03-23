// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/ItemBase.h"
#include "EquipmentsWeaponAcc.generated.h"

UCLASS()
class PORTFOLIO_API AEquipmentsWeaponAcc : public AItemBase
{
	GENERATED_BODY()
	
	AEquipmentsWeaponAcc();
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void GetWeight(int32& Weight) override;
	FItemWeaponAcc* Datas;
	EWeaponAccType AccType;
	
private:
	UPROPERTY()
	UDataTable* ItemWeaponAccTable;
	
};
