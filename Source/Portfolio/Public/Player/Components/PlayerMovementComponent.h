// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

class APortfolioCharacter;

USTRUCT(BlueprintType)
struct FMovementData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMovementData() : Forward(175.0f), Other(158.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Other;
};

UCLASS()
class PORTFOLIO_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMovementComponent();
	FMovementData* GetMovementData(FName Rowname) const;
	virtual float GetMaxSpeed() const override;
	APortfolioCharacter* PortfolioCharacter;
	 
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	UDataTable* MovementTable;
	
public:
	
	FString GetRowName() const;

private:

	UPROPERTY(EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAcces = true))
	float MovementSpeed;
};
