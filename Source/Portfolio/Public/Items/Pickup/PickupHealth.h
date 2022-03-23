// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "PickupHealth.generated.h"

UCLASS()
class PORTFOLIO_API APickupHealth : public AItemPickupBase
{
	GENERATED_BODY()

	APickupHealth();

	virtual void BeginPlay() override;

public:
	virtual void GetWeight(int32& Weight) override;
	
	void UseItem(class APortfolioCharacter* Player, FName Item);
	
	FItemHealth* Datas;

	int32 SingleWeight;

	
private:
	UPROPERTY()
	UDataTable* ItemHealthTable;

};
