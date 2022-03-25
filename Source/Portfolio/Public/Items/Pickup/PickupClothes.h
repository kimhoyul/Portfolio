// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "PickupClothes.generated.h"

UCLASS()
class PORTFOLIO_API APickupClothes : public AItemPickupBase
{
	GENERATED_BODY()

	APickupClothes();

	virtual void BeginPlay() override;
	
public:
	FItemClothes* Datas;

	UPROPERTY(BlueprintReadOnly)
	FItemClothes DatasRef;
	
	EClothesType ClothesType;
	
private:
	UPROPERTY()
	UDataTable* ItemClothesTable;
};
