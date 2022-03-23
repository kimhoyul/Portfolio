// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/ItemBase.h"
#include "EquipmentsAmmo.generated.h"

UCLASS()
class PORTFOLIO_API AEquipmentsAmmo : public AItemBase
{
	GENERATED_BODY()

	AEquipmentsAmmo();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	virtual void GetWeight(int32 &Weight) override;
	FItemAmmo* Datas;
	
private:
	UPROPERTY()
	UDataTable* ItemAmmoTable;
};
