// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortfolioCharacterWidget.generated.h"

UCLASS()
class PORTFOLIO_API UPortfolioCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UPlayerStatComponent* CurrentCharacterStat;
	
};
