// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineTraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API ULineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULineTraceComponent();

protected:
	virtual void BeginPlay() override;

public:
	FHitResult LineTraceSingle(FVector Start, FVector End);
	FHitResult LineTraceSingle(FVector Start, FVector End, bool ShowDebugLine);

	FHitResult LineTraceSingleFire(FVector Start, FVector End);
	FHitResult LineTraceSingleFire(FVector Start, FVector End, bool ShowDebugLine);
};
