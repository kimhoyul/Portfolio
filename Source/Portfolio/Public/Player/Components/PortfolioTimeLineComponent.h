// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortfolioCharacter.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "PortfolioTimeLineComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UPortfolioTimeLineComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UPortfolioTimeLineComponent();

	UPROPERTY(EditAnywhere, Category = "Curve")
	UCurveFloat* FreeLookCurveFloat;

	UPROPERTY(EditAnywhere, Category = "Curve")
	UCurveFloat* CameraHeightCurveFloat;

	APortfolioCharacter* GetTimeLineOwner();
	
	UFUNCTION()
	void FreeLookTimeLineFloatReturn(float Value);

	UFUNCTION()
	void FreeLookTimeLineFinished();

	UFUNCTION()
	void CameraHeightTimeLineFloatReturn(float Value);

	UFUNCTION()
	void CameraHeightTimeLineFinished();

	void UpdateCameraHeight();
	void FreeLookTimeLineStart();

	FTimeline FreeLookTimeLine;
	FTimeline CameraHeightTimeLine;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

