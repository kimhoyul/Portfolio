// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

UCLASS()
class PORTFOLIO_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void MainWidgetCreate(TSubclassOf<UUserWidget> NewWidgetClass);
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> MainUI;

	UPROPERTY()
	UUserWidget* CurrentWidget;
};
