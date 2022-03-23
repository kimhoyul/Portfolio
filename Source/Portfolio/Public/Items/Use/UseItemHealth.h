// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Use/ItemUseBase.h"
#include "UseItemHealth.generated.h"

UCLASS()
class PORTFOLIO_API AUseItemHealth : public AItemUseBase
{
	GENERATED_BODY()
	
	AUseItemHealth();
	
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void GetWeight(int32& Weight) override;
	FItemHealth* Datas;
	
private:
	UPROPERTY()
	UDataTable* ItemHealthTable;
};
