// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "UI/PortfolioCharacterWidget.h"
#include "PortfolioCharacter.generated.h"

class APortfolioPlayerState;
class UPortfolioGameInstance;
class AEquipmentsWeapon;
class AItemPickupBase;
class AItemBase;

UCLASS(config=Game)
class APortfolioCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	APortfolioCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	//////////////////////////////////////////////////////////////////////////
	// SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeltalMesh")
	USkeletalMeshComponent* SKMHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeltalMesh")
	USkeletalMeshComponent* SKMUnderwearTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeltalMesh")
	USkeletalMeshComponent* SKMUnderwearBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeltalMesh")
	USkeletalMeshComponent* SKMClothTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeltalMesh")
	USkeletalMeshComponent* SKMClothBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeltalMesh")
	USkeletalMeshComponent* SKMWhole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeltalMesh")
	USkeletalMeshComponent* SKMShoes;

	UPROPERTY(BlueprintReadWrite, Category = "Clothes")
	UMaterialInstanceDynamic* SkinMatRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothes")
	UDataTable* ClothesTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothes")
	UTexture* NoneOpacityTexture;

	UPROPERTY()
	USkeletalMesh* SKMRef;

	UPROPERTY()
	UTexture* Mask;
	
	UFUNCTION(BlueprintCallable)
	void ReplaceSkeletalMesh(EClothesType ClothesType, FName RowName);

	FItemClothes* GetClothesData(FName RowName) const;
	
	/** get aim offsets */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	FRotator GetAimOffsets() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TimeLine", meta = (AllowPrivateAccess = "true"))
	class UPortfolioTimeLineComponent* TimeLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	class UPlayerStatComponent* PlayerStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LineTrace", meta = (AllowPrivateAccess = "true"))
	class ULineTraceComponent* LineTraceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LineTrace", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseSensitive")
	float MouseSensitiveX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseSensitive")
	float MouseSensitiveY;
	
	UPROPERTY(BlueprintReadOnly,  Category = "Move")
	float WalkSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Move")
	bool WalkPressed;

	UPROPERTY(BlueprintReadWrite, Category = "Move")
	bool SprintPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPose")
	bool IsHoldWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPose")
	bool bIsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPose")
	bool IsPlayingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPose")
	bool bIsReload;
	
	UPROPERTY(Transient)
	float MoveForwardAxis;
	
	UPROPERTY(Transient)
	float MoveRightAxis;

	//////////////////////////////////////////////////////////////////////////
	// Timer Handle
	FTimerHandle LineTraceHandle;
	FTimerHandle RunningTimeHandle;
	FTimerHandle DestroyHandle;
	
	//////////////////////////////////////////////////////////////////////////
	// FreeLook
	void FreeLookStart();
	void FreeLookStop();
	void FreeLookFloat(float Value);
	void FreeLookReset();
	
	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "CharacterPose")
	bool UseFreeLock;

	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "CharacterPose")
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
	void MouseTurn(float AxisValue);
	void MouseLookUp(float AxisValue);
	void OnStartAiming();
	void OnStopAiming();
	void OnToggleInInventory();
	void Fire();
	void Interaction();

	UFUNCTION(BlueprintCallable)
	void UIInteraction(AItemPickupBase* Item);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> InventoryWidgetClass;
	
	//////////////////////////////////////////////////////////////////////////
	// Pose
	
	UPROPERTY(BlueprintReadWrite, Transient, Replicated)
	bool EnableMove;

	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "CharacterPose")
	bool bWantsToAiming;

	//////////////////////////////////////////////////////////////////////////
	// Die
	UFUNCTION()
	void CallDestroy();

	UFUNCTION()
	void DIe();

	UPROPERTY()
	bool bIsDied;
	
	//////////////////////////////////////////////////////////////////////////
	// Crouching
	
	/** [server + local] change Crouching state */
	void SetCrouching(bool bNewCrouching, bool bIsProne);
	
	/** player toggled on crouch action */
	void OnStartCrouching();

	/** player toggled off crouch action */
	void OnStopCrouching();

	/** current running state */
	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "Character Pose")
	bool bWantsToCrouch;
	
	
	//////////////////////////////////////////////////////////////////////////
	// Prone

	/** [server + local] change Prone state */
	void SetProne(bool bNewProne, bool bIsProne, bool bIsCrouch);
	
	/** player toggled on Prone action */
	void OnStartProne();

	/** player toggled off Prone action */
	void OnStopProne();

	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "Character Pose")
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
	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "Character Pose")
	bool bWantsToRun;

	void HandleRunning();
	
protected:
	
	void InitSkeletalMesh();

	//////////////////////////////////////////////////////////////////////////
	// LineTrace
	UFUNCTION(	)
	void ItemLineTrace();

	

	UPROPERTY()
	AItemPickupBase* PointItem;
	
	
	//////////////////////////////////////////////////////////////////////////
	// Stat
	UFUNCTION(BlueprintPure)
	float ReturnPlayerHunger();

	UFUNCTION(BlueprintPure)
	float ReturnPlayerThirst();

	UFUNCTION(BlueprintPure)
	float ReturnPlayerHealth();
	
	UFUNCTION(BlueprintPure)
	float ReturnPlayerStamina();

	
	
	//////////////////////////////////////////////////////////////////////////
	// ChangeItems
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponDisplay(FName HoldSocket);
	
	UFUNCTION()
	void OnUpdateWeaponDisplay(AEquipmentsWeapon* Weapon, EWeaponPosition Position, bool IsOnHand);

	void UpdateEquipmentDisplay();
	
	UFUNCTION()
	void OnUpdateEquipmentDisplay(AItemBase* Equipment, bool IsAdd);
	
	void UpdateClothesDisplay();
	
	UFUNCTION()
	void OnUpdateClothesDisplay(AItemBase* Clothes, bool IsAdd);
	
	void Attach(AItemBase* Item, FName SocketName);

	void ClearClothes();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void CalculateHoldGunSocket(FName& SocketName);

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool Interrupted);
	

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

	/** update Aim state */
	UFUNCTION(Reliable, server, WithValidation)
	void ServerSetAimRotation(float Rotation);
	
	/** update LineTrace */
	UFUNCTION(Reliable, server, WithValidation)
	void ServerItemLineTrace(AItemPickupBase* PickupItem);

	/** update Fire */
	UFUNCTION(Reliable, server, WithValidation)
	void ServerFire(APortfolioCharacter* Actor);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastDIe();
	
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerInteraction();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MultiInteraction(AItemPickupBase* Item);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUIInteraction(AItemPickupBase* Item);

	UFUNCTION(BlueprintCallable)
	void EquipmentSpwan(FName ID, FString SN);
	
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquipmentSpwan(FName ID, const FString& SN);
	
	//////////////////////////////////////////////////////////////////////////
	// ForceInline
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UPortfolioTimeLineComponent* GetTimeLine() const { return TimeLine; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFrameworkRef")
	APortfolioPlayerState* PortfolioPlayerStateRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFrameworkRef")
	UPortfolioGameInstance* PortfolioGameInstanceRef;

private:
	UPROPERTY()
	class UUserWidget* InventoryWidget;

};





