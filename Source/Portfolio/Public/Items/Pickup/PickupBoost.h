// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "PickupBoost.generated.h"

UCLASS()
class PORTFOLIO_API APickupBoost : public AItemPickupBase
{
	GENERATED_BODY()
	
	APickupBoost();

	virtual void BeginPlay() override;

public:
	virtual void GetWeight(int32& Weight) override;

	FItemBoost* Datas;

	int32 SingleWeight;
	
private:
	UPROPERTY()
	UDataTable* ItemBoostTable;
};
