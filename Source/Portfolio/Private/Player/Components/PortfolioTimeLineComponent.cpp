// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/PortfolioTimeLineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PortfolioCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogPortfolioTimeLine, All, All);

UPortfolioTimeLineComponent::UPortfolioTimeLineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPortfolioTimeLineComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPortfolioTimeLineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FreeLookTimeLine.TickTimeline(DeltaTime);
	CameraHeightTimeLine.TickTimeline(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// CameraHeight
void UPortfolioTimeLineComponent::UpdateCameraHeight()
{
	if (CameraHeightCurveFloat)
	{
		FOnTimelineFloat InterpFunction;
		FOnTimelineEvent TimelineFinished;
		InterpFunction.BindUFunction(this, FName("CameraHeightTimeLineFloatReturn"));
		TimelineFinished.BindUFunction(this, FName("CameraHeightTimeLineFinished"));
		CameraHeightTimeLine.SetTimelineFinishedFunc(TimelineFinished);

		CameraHeightTimeLine.AddInterpFloat(CameraHeightCurveFloat, InterpFunction, FName("Alpha"));

		CameraHeightTimeLine.SetLooping(false);
		CameraHeightTimeLine.PlayFromStart();
	}
}

void UPortfolioTimeLineComponent::CameraHeightTimeLineFloatReturn(float Value)
{
	GetTimeLineOwner()->CameraHeightFloat(Value);
}

inline void UPortfolioTimeLineComponent::CameraHeightTimeLineFinished()
{
	GetTimeLineOwner()->CurrentHeight = GetTimeLineOwner()->NewHeight;
}


void UPortfolioTimeLineComponent::FreeLookTimeLineStart()
{
	if (FreeLookCurveFloat)
	{
		FOnTimelineFloat InterpFunction;
		FOnTimelineEvent TimelineFinished;
		InterpFunction.BindUFunction(this, FName("FreeLookTimeLineFloatReturn"));
		TimelineFinished.BindUFunction(this, FName("FreeLookTimeLineFinished"));

		FreeLookTimeLine.AddInterpFloat(FreeLookCurveFloat, InterpFunction, FName("Alpha"));
		FreeLookTimeLine.SetTimelineFinishedFunc(TimelineFinished);

		FreeLookTimeLine.SetLooping(false);
		FreeLookTimeLine.PlayFromStart();
	}
}

APortfolioCharacter* UPortfolioTimeLineComponent::GetTimeLineOwner() 
{
	APortfolioCharacter* CharacterOwner = Cast<APortfolioCharacter>(GetOwner());
	if (!CharacterOwner) return nullptr;
	return CharacterOwner;
}

void UPortfolioTimeLineComponent::FreeLookTimeLineFloatReturn(float Value)
{
	GetTimeLineOwner()->FreeLookFloat(Value);
}

void UPortfolioTimeLineComponent::FreeLookTimeLineFinished()
{
	GetTimeLineOwner()->FreeLookReset();
}