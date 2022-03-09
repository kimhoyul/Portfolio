// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortfolioGameMode.h"
#include "Player/PortfolioCharacter.h"
#include "UI/GameHUD.h"
#include "Player/PortfolioPlayerController.h"
#include "Player/PortfolioPlayerState.h"
#include "PortfolioGameStateBase.h"

APortfolioGameMode::APortfolioGameMode()
{
	DefaultPawnClass = APortfolioCharacter::StaticClass();
	PlayerControllerClass = APortfolioPlayerController::StaticClass();
	HUDClass = AGameHUD::StaticClass();
	PlayerStateClass = APortfolioPlayerState::StaticClass();
	GameStateClass = APortfolioGameStateBase::StaticClass();
}
