// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "PickupAmmo.generated.h"

class UPortfolioGameInstance;

UCLASS()
class PORTFOLIO_API APickupAmmo : public AItemPickupBase
{
	GENERATED_BODY()

	APickupAmmo();
	virtual void BeginPlay() override;

	virtual void GetWeight(int32& Weight) override;

public:
	FItemAmmo* Datas;

	UPROPERTY(BlueprintReadOnly)
	FItemAmmo DatasRef;
	
	UPROPERTY()
	FName IDS;

	UPROPERTY()
	UPortfolioGameInstance* GameInstanceRef;
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void ServerSpawn(FName IDRef);
	
private:
	UPROPERTY()
	int32 SingleWeight;
	
	UPROPERTY()
	UDataTable* ItemAmmoTable;

	

};

