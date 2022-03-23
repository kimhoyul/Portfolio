// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortfolioCharacter.h"

#include <vcruntime_startup.h>

#include "PortfolioAnimInstance.h"
#include "PortfolioGameInstance.h"
#include "PortfolioPlayerController.h"
#include "PortfolioPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PlayerMovementComponent.h"
#include "Components/PortfolioTimeLineComponent.h"
#include "Datas/EnumData.h"
#include "Datas/StructData.h"
#include "Engine/TimelineTemplate.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Equipments/EquipmentsClothes.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Components/InventoryComponent.h"
#include "Components/LineTraceComponent.h"
#include "Items/Pickup/PickupHealth.h"
#include "Player/Components/PlayerStatComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogPortfolioCharacter, All, All);

APortfolioCharacter::APortfolioCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	SKMHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMHair"));
	SKMHair->SetupAttachment(GetMesh());
	SKMHair->SetRelativeTransform(FTransform(FVector(1.0f, 1.0f, 1.0f)));

	SKMUnderwearTop = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMUnderwearTop"));
	SKMUnderwearTop->SetupAttachment(GetMesh());

	SKMUnderwearBottom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMUnderwearBottom"));
	SKMUnderwearBottom->SetupAttachment(GetMesh());

	SKMClothTop = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMClothTop"));
	SKMClothTop->SetupAttachment(GetMesh());

	SKMClothBottom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMClothBottom"));
	SKMClothBottom->SetupAttachment(GetMesh());

	SKMWhole = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMWhole"));
	SKMWhole->SetupAttachment(GetMesh());

	SKMShoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMShoes"));
	SKMShoes->SetupAttachment(GetMesh());
	
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

	PlayerStatComponent = CreateDefaultSubobject<UPlayerStatComponent>("PlayerStatComponent");

	LineTraceComponent = CreateDefaultSubobject<ULineTraceComponent>("LineTraceComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	MouseSensitiveX = 1.0f;
	MouseSensitiveY = 1.0f;
	WalkSpeed = 600.0f;

	bWantsToCrouch = false;
	bWantsToProne = false;
	bWantsToRun = false;
	bWantsToAiming = false;
	EnableMove = true;
	IsHoldWeapon = false;
	bIsDied = false;
	
	NewHeight = 0.0f;
	CurrentHeight = 90.0f;
	OriginalHeight = 0.0f;
}

void APortfolioCharacter::InitSkeletalMesh()
{
	auto SKM = GetMesh()->SkeletalMesh->GetMaterials()[2];
	auto MaterialInterface = SKM.MaterialInterface;
	auto SlotName = SKM.MaterialSlotName;
	SkinMatRef = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetOwner(), MaterialInterface, {},{});
	GetMesh()->SetMaterialByName(SlotName, SkinMatRef);
	
	SKMUnderwearTop->SetMasterPoseComponent(GetMesh(), false);
	SKMUnderwearBottom->SetMasterPoseComponent(GetMesh(), false);
	SKMClothTop->SetMasterPoseComponent(GetMesh(), false);
	SKMClothBottom->SetMasterPoseComponent(GetMesh(), false);
	SKMWhole->SetMasterPoseComponent(GetMesh(), false);
	SKMShoes->SetMasterPoseComponent(GetMesh(), false);

	SKMHair->K2_AttachToComponent(GetMesh(), FName("head"), EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);

	ReplaceSkeletalMesh(EClothesType::ECT_ClothTop, "1");
	ReplaceSkeletalMesh(EClothesType::ECT_ClothBottom, "4");
}

void APortfolioCharacter::ReplaceSkeletalMesh(EClothesType ClothesType, FName RowName)
{
	SKMRef= GetClothesData(RowName)->SkeletalMesh;
	Mask = GetClothesData(RowName)->MaskTexture;

	switch (ClothesType)
	{
	case  EClothesType::ECT_ClothTop :
		{
			SKMClothTop->SetSkeletalMesh(SKMRef, true);
			SkinMatRef->SetTextureParameterValue("Mask Top", Mask);
			if (SKMRef != nullptr)
			{
				SKMWhole->SetSkeletalMesh(nullptr, true);
			}
			break;
		}
	case  EClothesType::ECT_ClothBottom :
		{
			SKMClothBottom->SetSkeletalMesh(SKMRef, true);
			SkinMatRef->SetTextureParameterValue("Mask Bottom", Mask);
			if (SKMRef != nullptr)
			{
				SKMWhole->SetSkeletalMesh(nullptr, true);
			}
			break;
		}
	case  EClothesType::ECT_Whole :
		{
			SKMWhole->SetSkeletalMesh(SKMRef, true);
			SkinMatRef->SetTextureParameterValue("Mask Top", Mask);
			if (SKMRef != nullptr)
			{
				SKMClothTop->SetSkeletalMesh(nullptr, true);
				SKMClothBottom->SetSkeletalMesh(nullptr, true);
			}
			break;
		}
	case  EClothesType::ECT_Shoes :
		{
			SKMShoes->SetSkeletalMesh(SKMRef, true);
			if (IsValid(SKMRef))
			{
				SkinMatRef->SetTextureParameterValue("Mask Foot", Mask);
				GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
			}
			else
			{
				SkinMatRef->SetTextureParameterValue("Mask Foot", NoneOpacityTexture);
				GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -98.0f));
			}
			break;
		}
	}

	if (IsValid(SKMWhole->SkeletalMesh))
	{
		SkinMatRef->SetTextureParameterValue("Mask Bottom", NoneOpacityTexture);
	}
	else
	{
		if (!IsValid(SKMClothTop->SkeletalMesh))
		{
			SKMClothTop->SetSkeletalMesh(GetClothesData("1")->SkeletalMesh, true);
			SkinMatRef->SetTextureParameterValue("Mask Top", GetClothesData("1")->MaskTexture);
		}

		if (!IsValid(SKMClothBottom->SkeletalMesh))
		{
			SKMClothBottom->SetSkeletalMesh(GetClothesData("4")->SkeletalMesh, true);
			SkinMatRef->SetTextureParameterValue("Mask Bottom", GetClothesData("4")->MaskTexture);
		}
	}
}

FItemClothes* APortfolioCharacter::GetClothesData(FName RowName) const
{
	return ClothesTable->FindRow<FItemClothes>(RowName, TEXT(""));
}

void APortfolioCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PortfolioPlayerStateRef = Cast<APortfolioPlayerState>(GetPlayerState());

	if (!PortfolioPlayerStateRef)
	{
		GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("APortfolioCharacter : PortfolioPlayerStateRef 생성 실패")));
		OnRep_PlayerState();
	}
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("APortfolioCharacter : PortfolioPlayerStateRef 생성 성공")));
	// }
}

void APortfolioCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitSkeletalMesh();

	if(HasAuthority())
	{
		PortfolioPlayerStateRef = Cast<APortfolioPlayerState>(GetPlayerState());
		if(!PortfolioPlayerStateRef)
		{
			PortfolioPlayerStateRef = Cast<APortfolioPlayerState>(GetPlayerState());
		}
	}
	
	PortfolioGameInstanceRef = GetWorld() != NULL ? Cast<UPortfolioGameInstance>(GetWorld()->GetGameInstance()) : NULL;
	
	if(PortfolioPlayerStateRef)
	{
		PortfolioPlayerStateRef->OnWeaponChanged.AddDynamic (this, &APortfolioCharacter::OnUpdateWeaponDisplay);
		PortfolioPlayerStateRef->OnEquipmentsChanged.AddDynamic (this, &APortfolioCharacter::OnUpdateEquipmentDisplay);
		PortfolioPlayerStateRef->OnClothesChanged.AddDynamic (this, &APortfolioCharacter::OnUpdateClothesDisplay);
	}
	
	UPortfolioAnimInstance* PortfolioAnimInstanceRef = Cast<UPortfolioAnimInstance>(GetMesh()->GetAnimInstance());
	PortfolioAnimInstanceRef->OnMontageEnded.AddDynamic(this, &APortfolioCharacter::OnMontageEnd);

	GetWorld()->GetTimerManager().SetTimer(RunningTimeHandle, this, &APortfolioCharacter::HandleRunning, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(LineTraceHandle, this, &APortfolioCharacter::ItemLineTrace, 0.1f, true);
}

void APortfolioCharacter::HandleRunning()
{
	if (bWantsToRun && this->GetVelocity().Size())
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			PlayerStatComponent->LowerStamina(10.0f);
		}
		if (PlayerStatComponent->GetStaminaRatio() <= 0.000f)
		{
			OnStopRunning();
		}
	}
}

void APortfolioCharacter::ItemLineTrace()
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start +(FollowCamera->GetForwardVector() * 450.0f);
	
	FHitResult HitResult = LineTraceComponent->LineTraceSingle(Start, End);//, true);
	if (AActor* Actor = HitResult.GetActor())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Hit Actor : %s"), *Actor->GetName()));
		if (AItemPickupBase* PickupItem = Cast<AItemPickupBase>(Actor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Actor is a Pickup")));
			ServerItemLineTrace(PickupItem);
		}
	}
}

void APortfolioCharacter::OnUpdateWeaponDisplay(AEquipmentsWeapon* Weapon, EWeaponPosition Position, bool IsOnHand)
{
	FName Sockname;
	CalculateHoldGunSocket(Sockname);
	UpdateWeaponDisplay(Sockname);
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
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &APortfolioCharacter::OnStartAiming);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &APortfolioCharacter::OnStopAiming);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &APortfolioCharacter::OnToggleInInventory);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APortfolioCharacter::Fire);
}

void APortfolioCharacter::OnToggleInInventory()
{
	GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("인입")));
}

void APortfolioCharacter::Fire()
{
	if (bIsDied) return;
	FVector Start = FollowCamera->GetComponentLocation() + (FollowCamera->GetForwardVector() * 300.0f);
	FVector End = FollowCamera->GetComponentLocation() +(FollowCamera->GetForwardVector() * 100000.0f);
	
	FHitResult HitResult = LineTraceComponent->LineTraceSingleFire(Start, End, true);
	if (AActor* Actor = HitResult.GetActor())
	{
		if (APortfolioCharacter* Player = Cast<APortfolioCharacter>(Actor))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Actor is a Pickup")));
			ServerFire(Player);
		}
	}
}

float APortfolioCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (GetLocalRole() < ROLE_Authority || PlayerStatComponent->GetHealthRatio() <= 0.0f) return 0.0f;
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if(ActualDamage > 0.0f)
	{
		PlayerStatComponent->LowerHealth(ActualDamage);
		if (PlayerStatComponent->GetHealthRatio() <= 0.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Actor is a Died")));
			DIe();
		}
	}
	return ActualDamage;
}

void APortfolioCharacter::DIe()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MulticastDIe();
	}
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

//////////////////////////////////////////////////////////////////////////
// Aiming
void APortfolioCharacter::OnStartAiming()
{
	if (IsHoldWeapon)
	{
		bWantsToAiming = true;
		FName SoketName;
		CalculateHoldGunSocket(SoketName);
		UpdateWeaponDisplay(SoketName);
	}
}

void APortfolioCharacter::OnStopAiming()
{
	bWantsToAiming = false;
	FName SoketName;
	CalculateHoldGunSocket(SoketName);
	UpdateWeaponDisplay(SoketName);
}


void APortfolioCharacter::MoveForward(float AxisValue)
{
	if (MoveForwardAxis != AxisValue)
	{
		MoveForwardAxis = AxisValue;
		UpdateCameraHeight();
		FName SocketName;
		CalculateHoldGunSocket(SocketName);
		UpdateWeaponDisplay(SocketName);
	}
	if (EnableMove)
	{
		
		Moving(true ,AxisValue);	
	}
}

void APortfolioCharacter::MoveRight(float AxisValue)
{
	if (MoveRightAxis != AxisValue)
	{
		MoveRightAxis = AxisValue;
		UpdateCameraHeight();
		FName SocketName;
		CalculateHoldGunSocket(SocketName);
		UpdateWeaponDisplay(SocketName);
	}
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
	UE_LOG(LogPortfolioCharacter, Warning, TEXT("누름"));
	UseFreeLock = false;
	bUseControllerRotationYaw = UseFreeLock;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetFreeLook(true);
	}
}

void APortfolioCharacter::FreeLookStop()
{
	UE_LOG(LogPortfolioCharacter, Warning, TEXT("땜"));
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
	FName SocketName;
	CalculateHoldGunSocket(SocketName);
	UpdateWeaponDisplay(SocketName);
	
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
	FName SocketName;
	CalculateHoldGunSocket(SocketName);
	UpdateWeaponDisplay(SocketName);
	
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
			if (PlayerStatComponent->GetStaminaRatio() <= 0.15f)
			{
				return;
			}
			APortfolioPlayerController* MyPC = Cast<APortfolioPlayerController>(Controller);
			if (MyPC && MyPC->IsGameInputAllowed())
			{
				PlayerStatComponent->LowerStamina(15.0f);
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
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%f"), PlayerStatComponent->GetStaminaRatio()));
	if (PlayerStatComponent->GetStaminaRatio() > 0.1f)
	{
		PlayerStatComponent->ControlRunningTimer(true);
		APortfolioPlayerController* MyPC = Cast<APortfolioPlayerController>(Controller);
		if (MyPC && MyPC->IsGameInputAllowed())
		{
			SetRunning(true);
		}
	}
	else if (PlayerStatComponent->GetStaminaRatio() <= 0.0f)
	{
		PlayerStatComponent->ControlRunningTimer(false);
	}
}

void APortfolioCharacter::OnStopRunning()
{
	PlayerStatComponent->ControlRunningTimer(false);
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

void APortfolioCharacter::UpdateWeaponDisplay(FName HoldSocket)
{
	bool IsEquipBackpack = false;
	//////////////////////////////////////////////////////////////////////////
	// Update the handheld weapon display
	if (HoldSocket != "None")
	{
		if (IsValid(PortfolioPlayerStateRef->GetHoldGun()))
		{
			Attach(PortfolioPlayerStateRef->GetHoldGun(), HoldSocket);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Update back weapon display
	for (AItemBase* ArrayElement : PortfolioPlayerStateRef->GetEquipments())
	{
		if (ArrayElement->ItemType == EItemType::EIT_Backpack)
		{
			IsEquipBackpack = true;
		}
	}
	
	if (IsValid(PortfolioPlayerStateRef->GetWeapon1()))
	{
		AEquipmentsWeapon* Item = PortfolioPlayerStateRef->GetWeapon1();
		if (IsEquipBackpack)
		{
			Attach(Item, PortfolioGameInstanceRef->BackLeftBName);
		}
		else
		{
			Attach(Item, PortfolioGameInstanceRef->BackLeftNName);
		}
	}

	if (IsValid(PortfolioPlayerStateRef->GetWeapon2()))
	{
		AEquipmentsWeapon* Item = PortfolioPlayerStateRef->GetWeapon2();
		if (IsEquipBackpack)
		{
			Attach(Item, PortfolioGameInstanceRef->BackRightBName);
		}
		else
		{
			Attach(Item, PortfolioGameInstanceRef->BackRightNName);
		}
	}
}

void APortfolioCharacter::OnUpdateEquipmentDisplay(AItemBase* Equipment, bool IsAdd)
{
	UpdateEquipmentDisplay();
	UpdateWeaponDisplay("None");
}

void APortfolioCharacter::UpdateEquipmentDisplay()
{
	AItemBase* Item;
	bool HasHelmet = false; 
	for (AItemBase* ArrayElement : PortfolioPlayerStateRef->GetEquipments())
	{
		Item = ArrayElement;
		switch (Item->ItemType)
		{
		case EItemType::EIT_Helmet :
			{
				Attach(Item, PortfolioGameInstanceRef->HelmetName);
				HasHelmet = true;
				break;
			}
		case EItemType::EIT_Vest :
			{
				Attach(Item, PortfolioGameInstanceRef->VestName);
				break;
			}
		case EItemType::EIT_Backpack :
			{
				Attach(Item, PortfolioGameInstanceRef->BackpackName);
				break;
			}
		}
	}

	if (HasHelmet)
	{
		SKMHair->SetVisibility(false, false);
	}
	else
	{
		SKMHair->SetVisibility(true, false);
	}
}

void APortfolioCharacter::OnUpdateClothesDisplay(AItemBase* Clothes, bool IsAdd)
{
	UpdateClothesDisplay();
}

void APortfolioCharacter::UpdateClothesDisplay()
{
	ClearClothes();
	for (AItemBase* ArrayElement : PortfolioPlayerStateRef->GetClothes())
	{
		AEquipmentsClothes* EquipmentsClothes = Cast<AEquipmentsClothes>(ArrayElement);
		ReplaceSkeletalMesh(EquipmentsClothes->ClothesType, EquipmentsClothes->ID);
	}
}

void APortfolioCharacter::Attach(AItemBase* Item, FName SocketName)
{
	FTransform SocketTransform = GetMesh()->GetSocketTransform(SocketName, RTS_World);
	Item->SetActorTransform(SocketTransform);
	Item->K2_AttachToComponent(GetMesh(), SocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, true);
}

void APortfolioCharacter::ClearClothes()
{
	ReplaceSkeletalMesh(EClothesType::ECT_ClothTop, "1");
	ReplaceSkeletalMesh(EClothesType::ECT_ClothBottom, "4");
}

void APortfolioCharacter::CalculateHoldGunSocket(FName& SocketName)
{
	FName GunSocket;

	//////////////////////////////////////////////////////////////////////////
	// Prone Gun Socket
	if (bWantsToProne)
	{
		if (MoveForwardAxis == 0.0f && MoveRightAxis == 0)
		{
			GunSocket = PortfolioGameInstanceRef->GunProneIdleName;
		}
		else 
		{
			if (MoveRightAxis == 0)
			{
				GunSocket = PortfolioGameInstanceRef->GunProneFBName;
			}
			else
			{
				GunSocket = PortfolioGameInstanceRef->GunProneOtherName;
			}
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// Crouch Gun Socket
		if (bWantsToCrouch && !bWantsToAiming)
		{
			GunSocket = PortfolioGameInstanceRef->GunCrouchName;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// Aim or Firing Gun Socket
			if(bWantsToAiming || bIsFiring)
			{
				GunSocket = PortfolioGameInstanceRef->GunAimName;
			}
			else
			{
				//////////////////////////////////////////////////////////////////////////
				// Reload Gun Socket
				if(bIsReload)
				{
					GunSocket = PortfolioGameInstanceRef->GunReloadName;
				}
				//////////////////////////////////////////////////////////////////////////
				// Stand, Jump, Transitional, Equip, UnEquip, Death
				else
				{
					GunSocket = PortfolioGameInstanceRef->GunStandName;
				}
			}
		}
	}

	SocketName = GunSocket;
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%s"), *FText::FromName(SocketName).ToString()));
}

void APortfolioCharacter::OnMontageEnd(UAnimMontage* Montage, bool Interrupted)
{
	if (!IsValid(GetCurrentMontage()))
	{
		UPortfolioAnimInstance* PortfolioAnimInstanceRef = Cast<UPortfolioAnimInstance>(GetMesh()->GetAnimInstance());
		PortfolioAnimInstanceRef->IsPlayingMontage = false;
	}
}

float APortfolioCharacter::ReturnPlayerHunger()
{
	return PlayerStatComponent->GetHungerRatio();
}

float APortfolioCharacter::ReturnPlayerThirst()
{
	return PlayerStatComponent->GetThirstRatio();
}

float APortfolioCharacter::ReturnPlayerHealth()
{
	return PlayerStatComponent->GetHealthRatio();
}

float APortfolioCharacter::ReturnPlayerStamina()
{
	return PlayerStatComponent->GetStaminaRatio();
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

void APortfolioCharacter::ServerItemLineTrace_Implementation(AItemPickupBase* PickupItem)
{
	if (APickupHealth* Pickup_Item = Cast<APickupHealth>(PickupItem))
	{
		Pickup_Item->UseItem(this, Pickup_Item->ID);
	}
}

bool APortfolioCharacter::ServerItemLineTrace_Validate(AItemPickupBase* PickupItem)
{
	return true;
}


void APortfolioCharacter::ServerFire_Implementation(APortfolioCharacter* Actor)
{
	if (APortfolioCharacter* Player = Cast<APortfolioCharacter>(Actor))
	{
		float TestDamage = 20.0f;
		Player->TakeDamage(TestDamage,  FDamageEvent(), GetController(), this);
		//GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("Fire!! : [%s]"), *Player->GetName()));
		//Player->PlayerStatComponent->LowerHealth(10.0f);
	}
}

bool APortfolioCharacter::ServerFire_Validate(APortfolioCharacter* Player)
{
	return true;
}

void APortfolioCharacter::MulticastDIe_Implementation()
{
	this->bIsDied = true;
	this->GetCharacterMovement()->DisableMovement();
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	this->GetMesh()->SetAllBodiesSimulatePhysics(true);
}

bool APortfolioCharacter::MulticastDIe_Validate()
{
	return true;
}