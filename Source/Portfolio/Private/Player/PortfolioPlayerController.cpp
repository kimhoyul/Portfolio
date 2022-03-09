// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PortfolioPlayerController.h"



DEFINE_LOG_CATEGORY_STATIC(LogPortfolioPlayerController, All, All);

APortfolioPlayerController::APortfolioPlayerController()
{
	
}

void APortfolioPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

bool APortfolioPlayerController::IsGameInputAllowed() const
{
	return !bCinematicMode;
}

void APortfolioPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
