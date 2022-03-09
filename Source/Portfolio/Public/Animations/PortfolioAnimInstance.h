// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortfolioCharacter.h"
#include "Animation/AnimInstance.h"
#include "PortfolioAnimInstance.generated.h"

UCLASS()
class PORTFOLIO_API UPortfolioAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPortfolioAnimInstance();
	
	APortfolioCharacter* PortfolioCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	float Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	float ForwardValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	float SidewayVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsProne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsHoldWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsAim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsFreeFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsParachuteOpened;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	bool IsPlayingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGraph")
	int32 AimOffsetType;
};
