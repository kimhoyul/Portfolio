// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VicinityListWidget.generated.h"

class AItemPickupBase;

UCLASS()
class PORTFOLIO_API UVicinityListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddItemInventoryWidget(AItemPickupBase* ItemPickupBase);
};
