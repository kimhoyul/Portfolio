// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortfolioCharacter.h"
#include "PortfolioPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PlayerMovementComponent.h"
#include "Components/PortfolioTimeLineComponent.h"
#include "Engine/TimelineTemplate.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogPortfolioCharacter, All, All);

APortfolioCharacter::APortfolioCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeTransform(FTransform(FVector(0.0f, 25.0f, 90.0f)));
	CameraBoom->TargetArmLength = 200.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	TimeLine = CreateDefaultSubobject<UPortfolioTimeLineComponent>(TEXT("TimeLine"));
	
	MouseSensitiveX = 1.0f;
	MouseSensitiveY = 1.0f;
	WalkSpeed = 600.0f;

	bWantsToCrouch = false;
	bWantsToProne = false;
	bWantsToRun = false;
	bWantsToAiming = false;
	EnableMove = true;
	IsHoldWeapon = false;
	
	NewHeight = 0.0f;
	CurrentHeight = 90.0f;
	OriginalHeight = 0.0f;
}

FRotator APortfolioCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void APortfolioCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APortfolioCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APortfolioCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APortfolioCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APortfolioCharacter::LookUp);
	PlayerInputComponent->BindAction("FreeLook", IE_Pressed, this, &APortfolioCharacter::FreeLookStart);
	PlayerInputComponent->BindAction("FreeLook", IE_Released, this, &APortfolioCharacter::FreeLookStop);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APortfolioCharacter::OnStartCrouching);
	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &APortfolioCharacter::OnStartProne);
	PlayerInputComponent->BindAction("Jump/Vault", IE_Pressed, this, &APortfolioCharacter::OnJump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APortfolioCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APortfolioCharacter::OnStopRunning);
}

void APortfolioCharacter::MouseTurn(float AxisValue)
{
	AddControllerYawInput(AxisValue);

	FreeLookYaw = GetAimOffsets().Yaw / 180.0f;
	ServerSetAimRotation(FreeLookYaw);
}

void APortfolioCharacter::MouseLookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void APortfolioCharacter::MoveForward(float AxisValue)
{
	if (EnableMove)
	{
		Moving(true ,AxisValue);	
	}
}

void APortfolioCharacter::MoveRight(float AxisValue)
{
	if (EnableMove)
	{
		Moving(false ,AxisValue);
	}
}

void APortfolioCharacter::Turn(float AxisValue)
{
	AxisValue *= MouseSensitiveX;
	MouseTurn(AxisValue);
}

void APortfolioCharacter::LookUp(float AxisValue)
{
	AxisValue *= MouseSensitiveY;
	if (LimitPitchAngle(AxisValue))
	{
		MouseLookUp(AxisValue);
	}
}

bool APortfolioCharacter::LimitPitchAngle(float Axis)
{
	float BottomValue;
	float TopValue;
	///////////1///////////
	if (bWantsToProne)
	{
		BottomValue = 350.0f;
		TopValue = 15.0f;
	}
	else
	{
		if (bWantsToCrouch)
		{
			BottomValue = 290.0f;
			TopValue = 50.0f;
		}
		else
		{
			BottomValue = 290.0f;
			TopValue = 75.0f;
		}
	}
	///////////2///////////
	const float Pitch = GetControlRotation().Pitch;
	
	if (!(Pitch > BottomValue || Pitch < TopValue))
	{
		const float ABSTop = UKismetMathLibrary::Abs(Pitch - TopValue);
		const float ABSBottom = UKismetMathLibrary::Abs(Pitch - BottomValue);
		
		if (ABSTop < ABSBottom)
		{
			const FRotator NewRotator = UKismetMathLibrary::MakeRotator(GetControlRotation().Roll, TopValue, GetControlRotation().Yaw);
			Controller->SetControlRotation(NewRotator);
		}
		else
		{
			const FRotator NewRotator = UKismetMathLibrary::MakeRotator(GetControlRotation().Roll, BottomValue, GetControlRotation().Yaw);
			Controller->SetControlRotation(NewRotator);
		}
	}
	///////////3///////////
	const float InputPitchScale = -2.5f;
	const float MultiScale = Axis * InputPitchScale;
	float PlusValue = Pitch + MultiScale;

	if (PlusValue > BottomValue || PlusValue < TopValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APortfolioCharacter::Moving(bool IsForward, float AxisValue)
{
	VelocitySmoothing();

	auto PlayerController = GetController();
	float Yaw = PlayerController->GetControlRotation().Yaw;
	FRotator Rotator = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, Yaw);
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Rotator);
	FVector RightVector = UKismetMathLibrary::GetRightVector(Rotator);
	if (IsForward)
	{
		AddMovementInput(ForwardVector, AxisValue);
	}
	else
	{
		AddMovementInput(RightVector, AxisValue);
	}
}

void APortfolioCharacter::VelocitySmoothing()
{
	FVector Velocity = GetCharacterMovement()->Velocity;
	float VectorLength = UKismetMathLibrary::VSize(Velocity);
	VectorLength += 5.0f;
	if (VectorLength <= WalkSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = VectorLength;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

//////////////////////////////////////////////////////////////////////////
// FreeLook
void APortfolioCharacter::FreeLookStart()
{
	UseFreeLock = false;
	bUseControllerRotationYaw = UseFreeLock;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetFreeLook(true);
	}
}

void APortfolioCharacter::FreeLookStop()
{
	if(!TimeLine) return ;
	TimeLine->FreeLookTimeLineStart();
}

void APortfolioCharacter::FreeLookFloat(float Value)
{
	Controller->SetControlRotation(UKismetMathLibrary::RLerp(GetControlRotation(), GetActorRotation(), Value, true));
}

void APortfolioCharacter::FreeLookReset()
{
	UseFreeLock = true;
	bUseControllerRotationYaw = UseFreeLock;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetFreeLook(false);
	}
}

//////////////////////////////////////////////////////////////////////////
// CameraHeight
void APortfolioCharacter::UpdateCameraHeight()
{
	if (bWantsToCrouch)
	{
		bool UseInputMoveAndNormal = (GetInputAxisValue(FName("MoveForward")) != 0.0f || GetInputAxisValue(FName("MoveRight")) != 0.0f) && !IsHoldWeapon;
		bool UseInputMoveAndRifle = (GetInputAxisValue(FName("MoveForward")) != 0.0f || GetInputAxisValue(FName("MoveRight")) != 0.0f) && IsHoldWeapon;
		UseInputMoveAndNormal ? NewHeight = 60.0f : UseInputMoveAndRifle ? NewHeight = 80.0f : NewHeight = 40.0f;
	}
	else
	{
		bWantsToProne ? NewHeight = -22.0f : NewHeight = 90.0f;
	}

	if (CurrentHeight != NewHeight)
	{
		if(!TimeLine) return ;
		TimeLine->UpdateCameraHeight();
	}
}

void APortfolioCharacter::CameraHeightFloat(float Value)
{
	OriginalHeight = CurrentHeight;
	float LocationX = CameraBoom->GetRelativeLocation().X;
	float LocationY = CameraBoom->GetRelativeLocation().Y;
	CameraBoom->SetRelativeLocation(FVector(LocationX, LocationY, UKismetMathLibrary::Lerp(OriginalHeight, NewHeight, Value)));
}

//////////////////////////////////////////////////////////////////////////
// Crouching

void APortfolioCharacter::OnStartCrouching()
{
	APortfolioPlayerController* MyPC = Cast<APortfolioPlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (bWantsToProne)
		{
			SetCrouching(true, true);
		}
		else if (bWantsToCrouch)
		{
			OnStopCrouching();
		}
		else
		{
			SetCrouching(true, false);
		}
	}
}

void APortfolioCharacter::OnStopCrouching()
{
	APortfolioPlayerController* MyPC = Cast<APortfolioPlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		SetCrouching(false, false);
	}
}

void APortfolioCharacter::SetCrouching(bool bNewCrouching, bool bIsProne)
{
	bWantsToCrouch = bNewCrouching;
	
	if (bIsProne)
	{
		OnStopProne();
		EnableMove = false;
		FTimerHandle WaitHandle;
		float WaitTime = 1.267f; 
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&](){EnableMove = true;}), WaitTime, false); 
	}
	
	LimitPitchAngle(0);
	UpdateCameraHeight();
	
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetCrouching(bNewCrouching, bIsProne);
	}
}

//////////////////////////////////////////////////////////////////////////
// Prone

void APortfolioCharacter::OnStartProne()
{
	APortfolioPlayerController* MyPC = Cast<APortfolioPlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (bWantsToProne)
		{
			OnStopProne();
		}
		else if (bWantsToCrouch)
		{
			SetProne(true,false, true);
		}
		else
		{
			SetProne(true, false, false);
		}
	}
}

void APortfolioCharacter::OnStopProne()
{
	APortfolioPlayerController* MyPC = Cast<APortfolioPlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		SetProne(false,true, false);
	}
}

void APortfolioCharacter::SetProne(bool bNewProne, bool bIsProne, bool bIsCrouch)
{
	bWantsToProne = bNewProne;
	if (bIsProne)
	{
		EnableMove = false;
		FTimerHandle WaitHandle;
		float WaitTime = 1.400f; 
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&](){EnableMove = true;}), WaitTime, false);
	}
	
	if (bIsCrouch)
	{
		OnStopCrouching();
		bWantsToAiming = false;
		EnableMove = false;
		FTimerHandle WaitHandle;
		float WaitTime = 0.967f; 
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&](){EnableMove = true;}), WaitTime, false);
	}

	if (bNewProne && !bIsProne && !bIsCrouch)
	{
		bWantsToAiming = false;
		EnableMove = false;
		FTimerHandle WaitHandle;
		float WaitTime = 1.167f; 
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&](){EnableMove = true;}), WaitTime, false); 
	}
	
	LimitPitchAngle(0);
	UpdateCameraHeight();
	
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetProne(bNewProne, bIsProne, bIsCrouch);
	}
}

//////////////////////////////////////////////////////////////////////////
// OnJump

void APortfolioCharacter::OnJump()
{
	if (EnableMove)
	{
		if (bWantsToProne)
		{
			SetCrouching(true, true);
		}
		else if (bWantsToCrouch)
		{
			OnStopCrouching();
		}
		else
		{
			APortfolioPlayerController* MyPC = Cast<APortfolioPlayerController>(Controller);
			if (MyPC && MyPC->IsGameInputAllowed())
			{
				Jump();
			}
		}
		UpdateCameraHeight();
	}
}

//////////////////////////////////////////////////////////////////////////
// Running

void APortfolioCharacter::OnStartRunning()
{
	APortfolioPlayerController* MyPC = Cast<APortfolioPlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		SetRunning(true);
	}
}

void APortfolioCharacter::OnStopRunning()
{
	SetRunning(false);
}

void APortfolioCharacter::SetRunning(bool bNewRunning)
{
	bWantsToRun = bNewRunning;

	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetRunning(bNewRunning);
	}
}
//////////////////////////////////////////////////////////////////////////
// Replication

void APortfolioCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 로컬 소유자를 제외한 모든 사람: 플래그 변경은 로컬에서 시작됩니다.
	DOREPLIFETIME_CONDITION(APortfolioCharacter, bWantsToRun, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APortfolioCharacter, bWantsToCrouch, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APortfolioCharacter, bWantsToProne, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APortfolioCharacter, bWantsToAiming, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APortfolioCharacter, EnableMove, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APortfolioCharacter, UseFreeLock, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APortfolioCharacter, FreeLookYaw, COND_SkipOwner);
}

void APortfolioCharacter::ServerSetCrouching_Implementation(bool bNewCrouching, bool bIsProne)
{
	SetCrouching(bNewCrouching, bIsProne);
}

bool APortfolioCharacter::ServerSetCrouching_Validate(bool bNewCrouching, bool bIsProne)
{
	return true;
}

inline void APortfolioCharacter::ServerSetProne_Implementation(bool bNewProne, bool bIsProne, bool bIsCrouch)
{
	SetProne(bNewProne, bIsProne, bIsCrouch);
}

inline bool APortfolioCharacter::ServerSetProne_Validate(bool bNewProne, bool bIsProne, bool bIsCrouch)
{
	return true;
}

void APortfolioCharacter::ServerSetRunning_Implementation(bool bNewRunning)
{
	SetRunning(bNewRunning);
}

bool APortfolioCharacter::ServerSetRunning_Validate(bool bNewRunning)
{
	return true;
}

void APortfolioCharacter::ServerSetFreeLook_Implementation(bool FreeLook)
{
	FreeLook ? FreeLookStart() : FreeLookReset();
}

bool APortfolioCharacter::ServerSetFreeLook_Validate(bool FreeLook)
{
	return true;
}

void APortfolioCharacter::ServerSetAimRotation_Implementation(float Rotation)
{
	FreeLookYaw = Rotation;
}

bool APortfolioCharacter::ServerSetAimRotation_Validate(float Rotation)
{
	return true;
}