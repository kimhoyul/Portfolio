// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/EnumData.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class APortfolioCharacter;
class APortfolioPlayerController;

UCLASS()
class PORTFOLIO_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:

	virtual void GetWeight(int32 &Weight);
	void UpdateAmount(bool bAdd, bool bReduce, int32 AmountRef);
	
	UPROPERTY(BlueprintReadWrite)
	EItemType ItemType;
	
	UPROPERTY(BlueprintReadWrite, Replicated, meta = (ExposeOnSpawn="true"))
	FName ID;
	
	UPROPERTY(BlueprintReadWrite, Replicated, meta = (ExposeOnSpawn="true"))
	FString SN;

	UPROPERTY(BlueprintReadWrite, Replicated, meta = (ExposeOnSpawn="true"))
	int32 Amount;
	
	FText Name;

protected:
	
	virtual void BeginPlay() override;

	void Init(EItemType ItemTypeRef, FText NameRef);

	UPROPERTY()
	APortfolioCharacter* PortfolioCharacter;

	UPROPERTY()
	APortfolioPlayerController* PortfolioPlayerController;
};
