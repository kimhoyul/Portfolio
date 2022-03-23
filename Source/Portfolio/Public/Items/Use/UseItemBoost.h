// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Use/ItemUseBase.h"
#include "UseItemBoost.generated.h"

UCLASS()
class PORTFOLIO_API AUseItemBoost : public AItemUseBase
{
	GENERATED_BODY()

	AUseItemBoost();
	
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void GetWeight(int32& Weight) override;
	FItemBoost* Datas;
	
private:
	UPROPERTY()
	UDataTable* ItemBoostTable;
};
