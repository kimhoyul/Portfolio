// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PortfolioAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "PortfolioCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAniminstance, All, All);

UPortfolioAnimInstance::UPortfolioAnimInstance()
{
	AimOffsetType = 0;
	Speed = 0.0f;	
	Direction = 0.0f;	
	Pitch = 0.0f;
	Yaw = 0.0f;
	ForwardValue = 0.0f;
	SidewayVelocity = 0.0f;
	IsCrouch = false;
	IsProne = false;
	IsHoldWeapon = false;
	IsAim = false;
	IsInAir = false;
	IsFreeFalling = false;
	IsParachuteOpened = false;
	IsDead = false;
	IsPlayingMontage = false;
}

void UPortfolioAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	PortfolioCharacter = Cast<APortfolioCharacter>(TryGetPawnOwner());
	if (!PortfolioCharacter) NativeBeginPlay();
}

void UPortfolioAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!PortfolioCharacter) return;
	//////////////////////////////////////////////////////////////////////////
	// Speed & Direction

	
	Speed = PortfolioCharacter->GetVelocity().Size();
	if (Speed > 0.0f)
	{
		FRotator VectorToRotarion = UKismetMathLibrary::Conv_VectorToRotator(PortfolioCharacter->GetVelocity());
		FRotator InvertRotation = UKismetMathLibrary::NegateRotator(PortfolioCharacter->GetActorRotation());
		float ComposeRotationYaw = UKismetMathLibrary::ComposeRotators(VectorToRotarion, InvertRotation).Yaw;
		
		if (ComposeRotationYaw >= 180.0f)
		{
			Direction = ComposeRotationYaw - 360.0f;
		}
		else
		{
			Direction = ComposeRotationYaw;
		}
	}
	else
	{
		Direction = 0.0f;
	}

	//////////////////////////////////////////////////////////////////////////
	// Aim Pitch & Yaw
	
	Pitch = PortfolioCharacter->GetAimOffsets().Pitch / 180.0f;
	Yaw = PortfolioCharacter->FreeLookYaw;

	//////////////////////////////////////////////////////////////////////////
	// AimOffset Type
	if (IsInAir || IsProne)
	{
		AimOffsetType = 0;
	}
	else 
	{
		if (IsCrouch)
		{
			if (IsHoldWeapon)
			{
				if (IsAim)
				{
					AimOffsetType = 6;
				}
				else
				{
					AimOffsetType = 4;
				}
			}
			else
			{
				AimOffsetType = 2;
			}
		}
		else
		{
			if (IsHoldWeapon)
			{
				if (IsAim)
				{
					AimOffsetType = 5;
				}
				else
				{
					AimOffsetType = 3;
				}
			}
			else
			{
				AimOffsetType = 1;
			}
		}
	}
		
	
	//////////////////////////////////////////////////////////////////////////
	// Ground Character Pose
	
	IsCrouch = PortfolioCharacter->bWantsToCrouch;
	IsProne = PortfolioCharacter->bWantsToProne;
	IsAim = PortfolioCharacter->bWantsToAiming;
	IsHoldWeapon = PortfolioCharacter->IsHoldWeapon;
	//IsDead
	IsPlayingMontage = PortfolioCharacter->IsPlayingMontage;

	//////////////////////////////////////////////////////////////////////////
	// Air Character Pose

	IsInAir = PortfolioCharacter->GetMovementComponent()->IsFalling();
	//IsFreeFalling
	//IsParachuteOpened
	//ForwardValue
	SidewayVelocity = UKismetMathLibrary::LessLess_VectorRotator(PortfolioCharacter->GetVelocity(), PortfolioCharacter->GetActorRotation()).Y;
}
