// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();
	
	virtual void BeginPlay() override;
	
	/////////////////////////////////////////////////////////////////////////
	// Set Player Stat
	void HandleHungerAndThirst();
	
	void AddHunger(float Value);
	void LowerHunger(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLowerHunger(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLowerHealth(float Value);
	
	void AddThirst(float Value);
	void LowerThirst(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLowerThirst(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLowerStamina(float Value);
	
	void AddHealth(float Value);
	void LowerHealth(float Value);
	
	void RegenerateStamina();
	void LowerStamina(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerControlRunning(bool IsRunning);
		
	/////////////////////////////////////////////////////////////////////////
	// Get Player Stat 
	float GetHungerRatio();
	float GetThirstRatio();
	float GetHealthRatio();
	float GetStaminaRatio();
	void ControlRunningTimer(bool IsRunning);

protected:
	FTimerHandle TimerHandle;
	FTimerHandle StaminaHandle;
	
	UPROPERTY(Replicated)
	float Hunger;

	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	float HungerDecrementValue;
	
	UPROPERTY(Replicated)
	float Thirst;

	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	float ThirstDecrementValue;
	
	UPROPERTY(Replicated)
	float Health;
	
	UPROPERTY(Replicated)
	float Stamina;
};
