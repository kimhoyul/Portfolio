// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/ItemBase.h"
#include "EquipmentsEquipment.generated.h"

UCLASS()
class PORTFOLIO_API AEquipmentsEquipment : public AItemBase
{
	GENERATED_BODY()

	AEquipmentsEquipment();
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	FItemEquipment* Datas;
	
private:
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	UDataTable* ItemEquipmentsTable;
};
