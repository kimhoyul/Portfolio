// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/PlayerMovementComponent.h"
#include "PortfolioCharacter.h"
#include "Kismet/KismetStringLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogMovementComponent, All, All);



UPlayerMovementComponent::UPlayerMovementComponent()
{
	bWantsInitializeComponent = true;
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	PortfolioCharacter = Cast<APortfolioCharacter>(PawnOwner);
	check(PortfolioCharacter);
}

void UPlayerMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}

float UPlayerMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	int16 speed = GetMovementData(UKismetStringLibrary::Conv_StringToName(GetRowName()))->Forward;
	//UE_LOG(LogMovementComponent, Warning, TEXT("GetRowName = %s"), *GetRowName());
	//UE_LOG(LogMovementComponent, Warning, TEXT("Speed = %d"), speed);
	UE_LOG(LogMovementComponent, Warning, TEXT("Speed = %f"), PortfolioCharacter->GetInputAxisValue(FName("MoveForward")));
	
	return MaxSpeed = speed;
}

FMovementData* UPlayerMovementComponent::GetMovementData(FName Rowname) const
{
		return MovementTable->FindRow<FMovementData>(Rowname, TEXT(""));
}

FString UPlayerMovementComponent::GetRowName() const
{
	if (PortfolioCharacter)
	{
		FString RawName;

		FString HoldWeapon = UKismetStringLibrary::Conv_BoolToString(PortfolioCharacter->IsHoldWeapon);
		RawName = UKismetStringLibrary::Concat_StrStr(*RawName, *HoldWeapon);

		if (PortfolioCharacter->bWantsToCrouch)
		{
			RawName = UKismetStringLibrary::Concat_StrStr(*RawName, *FString("_crouch"));
		}
		else
		{
			if (PortfolioCharacter->bWantsToProne)
			{
				RawName = UKismetStringLibrary::Concat_StrStr(*RawName, *FString("_prone"));
			}
			else
			{
				RawName = UKismetStringLibrary::Concat_StrStr(*RawName, *FString("_stand"));
			}
			
		}

		if (PortfolioCharacter->bWantsToAiming)
		{
			RawName = UKismetStringLibrary::Concat_StrStr(*RawName, *FString("_aim"));
		}
		else
		{
			if (PortfolioCharacter->WalkPressed)
			{
				RawName = UKismetStringLibrary::Concat_StrStr(*RawName, *FString("_walk"));
			}
			else
			{
				if (PortfolioCharacter->bWantsToRun)
				{
					RawName = UKismetStringLibrary::Concat_StrStr(*RawName, *FString("_run"));
				}
				else
				{
					RawName = UKismetStringLibrary::Concat_StrStr(*RawName, *FString("_jog"));
				}
			}
		}
		return RawName;
	}
	return {};
}
