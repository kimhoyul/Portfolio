// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "PortfolioCharacter.generated.h"

UCLASS(config=Game)
class APortfolioCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	APortfolioCharacter(const FObjectInitializer& ObjectInitializer);

	/** get aim offsets */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	FRotator GetAimOffsets() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UPortfolioTimeLineComponent* TimeLine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseSensitive")
	float MouseSensitiveX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseSensitive")
	float MouseSensitiveY;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,  Category = "Move")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool WalkPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool SprintPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPose")
	bool IsHoldWeapon;

	void MouseTurn(float AxisValue);
	void MouseLookUp(float AxisValue);

	//////////////////////////////////////////////////////////////////////////
	// FreeLook
	void FreeLookStart();
	void FreeLookStop();
	void FreeLookFloat(float Value);
	void FreeLookReset();
	/** current running state */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Replicated, Category = "Character Pose")
	bool UseFreeLock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Replicated, Category = "CharacterPose")
	float FreeLookYaw;

	//////////////////////////////////////////////////////////////////////////
	// CameraHeight
	void UpdateCameraHeight();
	void CameraHeightFloat(float Value);
	float NewHeight;
	float CurrentHeight;
	float OriginalHeight;

	//////////////////////////////////////////////////////////////////////////
	// Input handlers
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	void Moving(bool IsForward, float AxisValue);
	void VelocitySmoothing();
	bool LimitPitchAngle(float Axis);

	//////////////////////////////////////////////////////////////////////////
	// Pose
	
	UPROPERTY(Transient, Replicated)
	bool EnableMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Replicated, Category = "CharacterPose")
	bool bWantsToAiming;
	
	//////////////////////////////////////////////////////////////////////////
	// Crouching
	
	/** [server + local] change Crouching state */
	void SetCrouching(bool bNewCrouching, bool bIsProne);
	
	/** player toggled on crouch action */
	void OnStartCrouching();

	/** player toggled off crouch action */
	void OnStopCrouching();

	/** current running state */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Replicated, Category = "Character Pose")
	bool bWantsToCrouch;
	
	
	//////////////////////////////////////////////////////////////////////////
	// Prone

	/** [server + local] change Prone state */
	void SetProne(bool bNewProne, bool bIsProne, bool bIsCrouch);
	
	/** player toggled on Prone action */
	void OnStartProne();

	/** player toggled off Prone action */
	void OnStopProne();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Replicated, Category = "Character Pose")
	bool bWantsToProne;

	
	//////////////////////////////////////////////////////////////////////////
	// Jump
	
	/** player pressed jump action */
	void OnJump();
	
	//////////////////////////////////////////////////////////////////////////
	// Running

	/** [server + local] change running state */
	void SetRunning(bool bNewRunning);
	
	/** player pressed run action */
	void OnStartRunning();

	/** player released run action */
	void OnStopRunning();
	
	/** current running state */
	UPROPERTY(BlueprintReadOnly, Transient, Replicated)
	bool bWantsToRun;
	
protected:

	//////////////////////////////////////////////////////////////////////////
	// Replication

	/** update Crouching state */
	UFUNCTION(Reliable, server, WithValidation)
	void ServerSetCrouching(bool bNewCrouching, bool bIsProne);
	
	/** update Prone state */
	UFUNCTION(Reliable, server, WithValidation)
	void ServerSetProne(bool bNewProne, bool bIsProne, bool bIsCrouch);
	
	/** update Running state */
	UFUNCTION(Reliable, server, WithValidation)
	void ServerSetRunning(bool bNewRunning);

	/** update FreeLook state */
	UFUNCTION(Reliable, server, WithValidation)
	void ServerSetFreeLook(bool FreeLook);

	/** update FreeLook state */
	UFUNCTION(Reliable, server, WithValidation)
	void ServerSetAimRotation(float Rotation);

	//////////////////////////////////////////////////////////////////////////
	// ForceInline
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UPortfolioTimeLineComponent* GetTimeLine() const { return TimeLine; }
};


