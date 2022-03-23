// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/PlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"


UPlayerStatComponent::UPlayerStatComponent()
{
	Hunger = 100.0f;
	Thirst = 100.0f;
	Health = 100.0f;
	Stamina = 100.0f;
	HungerDecrementValue = 0.3f;
	ThirstDecrementValue = 0.5f;
}

float UPlayerStatComponent::GetHungerRatio()
{
	return (100 < KINDA_SMALL_NUMBER) ? 0.0f : (Hunger / 100.0f);
}

float UPlayerStatComponent::GetThirstRatio()
{
	return (100 < KINDA_SMALL_NUMBER) ? 0.0f : (Thirst / 100.0f);
}

float UPlayerStatComponent::GetHealthRatio()
{
	return (100 < KINDA_SMALL_NUMBER) ? 0.0f : (Health / 100.0f);
}

float UPlayerStatComponent::GetStaminaRatio()
{
	return (100 < KINDA_SMALL_NUMBER) ? 0.0f : (Stamina / 100.0f);
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	if (GetOwnerRole() == ROLE_Authority)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlayerStatComponent::HandleHungerAndThirst, 3.0f, true);
		GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &UPlayerStatComponent::RegenerateStamina, 1.0f, true);	
	}
}

void UPlayerStatComponent::HandleHungerAndThirst()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
	LowerHunger(HungerDecrementValue);
	LowerThirst(ThirstDecrementValue);
	}
}

void UPlayerStatComponent::AddHunger(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if(Hunger+Value >= 100.0f)
		{
			Hunger = 100.0f;
		}
		else
		{
			Hunger += Value;
		}
	}
}

void UPlayerStatComponent::LowerHunger(float Value)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerLowerHunger(Value);
	}
	else if (GetOwnerRole() == ROLE_Authority)
	{
		Hunger -= Value;
	}
}

void UPlayerStatComponent::AddThirst(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if(Thirst+Value >= 100.0f)
		{
			Thirst = 100.0f;
		}
		else
		{
			Thirst += Value;
		}
	}
}

void UPlayerStatComponent::LowerThirst(float Value)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerLowerThirst(Value);
	}
	else if (GetOwnerRole() == ROLE_Authority)
	{
		Thirst -= Value;
	}
}

void UPlayerStatComponent::AddHealth(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if(Health+Value >= 100.0f)
		{
			Health = 100.0f;
		}
		else
		{
			Health += Value;
		}
	}
}

void UPlayerStatComponent::LowerHealth(float Value)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerLowerHealth(Value);
	}
	else if (GetOwnerRole() == ROLE_Authority)
	{
		if (Health - Value <= 0.0f)
		{
			Health = 0.0f;
		}
		else
		{
			Health -= Value;
		}
	}
}

void UPlayerStatComponent::LowerStamina(float Value)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerLowerStamina(Value);
	}
	else if (GetOwnerRole() == ROLE_Authority)
	{
		Stamina -= Value;
	}
}

void UPlayerStatComponent::RegenerateStamina()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		Stamina >= 100.0f ? Stamina = 100.0f : Stamina += 15;
	}
}

void UPlayerStatComponent::ControlRunningTimer(bool IsRunning)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerControlRunning(IsRunning);
	}
	else
	{
		if (IsRunning)
		{
			GetWorld()->GetTimerManager().PauseTimer(StaminaHandle);
		}
		else
		{
			GetWorld()->GetTimerManager().UnPauseTimer(StaminaHandle);
		}
	}
}

void UPlayerStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UPlayerStatComponent, Hunger);
	DOREPLIFETIME(UPlayerStatComponent, Thirst);
	DOREPLIFETIME(UPlayerStatComponent, Health);
	DOREPLIFETIME(UPlayerStatComponent, Stamina);
}

void UPlayerStatComponent::ServerLowerHunger_Implementation(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		LowerHunger(Value);
	}
}

bool UPlayerStatComponent::ServerLowerHunger_Validate(float Value)
{
	return true;
}

void UPlayerStatComponent::ServerLowerThirst_Implementation(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		LowerThirst(Value);
	}
}

bool UPlayerStatComponent::ServerLowerThirst_Validate(float Value)
{
	return true;
}

void UPlayerStatComponent::ServerLowerHealth_Implementation(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		LowerHealth(Value);
	}
}

bool UPlayerStatComponent::ServerLowerHealth_Validate(float Value)
{
	return true;
}

void UPlayerStatComponent::ServerLowerStamina_Implementation(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		LowerStamina(Value);
	}
}

bool UPlayerStatComponent::ServerLowerStamina_Validate(float Value)
{
	return true;
}


void UPlayerStatComponent::ServerControlRunning_Implementation(bool IsRunning)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ControlRunningTimer(IsRunning);
	}
}

bool UPlayerStatComponent::ServerControlRunning_Validate(bool IsRunning)
{
	return true;
}