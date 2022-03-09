// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PortfolioPlayerController.generated.h"

class APortfolioCharacter;

UCLASS()
class PORTFOLIO_API APortfolioPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APortfolioPlayerController();
	
	virtual void BeginPlay() override;

	/** 게임 플레이 관련 작업(이동, 무기 사용 등)이 현재 허용되는지 확인 */
	bool IsGameInputAllowed() const;
	
protected:
	
	virtual void SetupInputComponent() override;

};
