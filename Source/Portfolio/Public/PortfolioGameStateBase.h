// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PortfolioGameStateBase.generated.h"

UCLASS()
class PORTFOLIO_API APortfolioGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	UPROPERTY()
	int32 LiveAmount;
};
