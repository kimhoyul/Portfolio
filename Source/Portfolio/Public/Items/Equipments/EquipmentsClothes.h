// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/ItemBase.h"
#include "EquipmentsClothes.generated.h"

UCLASS()
class PORTFOLIO_API AEquipmentsClothes : public AItemBase
{
	GENERATED_BODY()

	AEquipmentsClothes();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	FItemClothes* Datas;

	UPROPERTY(BlueprintReadOnly)
	EClothesType ClothesType;
	
private:
	UPROPERTY()
	UDataTable* ItemClothesTable;
};
