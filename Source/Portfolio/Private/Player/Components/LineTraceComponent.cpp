// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/LineTraceComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

ULineTraceComponent::ULineTraceComponent()
{
}

void ULineTraceComponent::BeginPlay()
{
	Super::BeginPlay();

}

FHitResult ULineTraceComponent::LineTraceSingle(FVector Start, FVector End)
{
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionParams;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		Start,
		End,
		CollisionParams,
		CollisionQueryParams
		))
	{
		return HitResult;
	}
	else
	{
		return HitResult;	
	}
}

FHitResult ULineTraceComponent::LineTraceSingle(FVector Start, FVector End, bool ShowDebugLine)
{
	FHitResult HitResult = LineTraceSingle(Start, End);
	if (ShowDebugLine)
	{
		DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Orange,
		false,
		0.2f
		);
	}
	return HitResult;
}

FHitResult ULineTraceComponent::LineTraceSingleFire(FVector Start, FVector End)
{
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionParams;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		Start,
		End,
		CollisionParams,
		CollisionQueryParams
		))
	{
		return HitResult;
	}
	else
	{
		return HitResult;	
	}
}

FHitResult ULineTraceComponent::LineTraceSingleFire(FVector Start, FVector End, bool ShowDebugLine)
{
	FHitResult HitResult = LineTraceSingle(Start, End);
	if (ShowDebugLine)
	{
		DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Red,
		false,
		1.2f,
		0,
		3.0f
		);
	}
	return HitResult;
}


