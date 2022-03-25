// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "PickupEquipment.generated.h"

UCLASS()
class PORTFOLIO_API APickupEquipment : public AItemPickupBase
{
	GENERATED_BODY()

	APickupEquipment();

	virtual void BeginPlay() override;

public:
	FItemEquipment* Datas;

	UPROPERTY(BlueprintReadOnly)
	FItemEquipment DatasRef;
	
private:
	EItemType EquipmentType;
	
	UPROPERTY()
	UDataTable* ItemEquipmentsTable;
};
