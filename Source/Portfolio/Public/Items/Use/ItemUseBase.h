// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "ItemUseBase.generated.h"

UCLASS()
class PORTFOLIO_API AItemUseBase : public AItemBase
{
	GENERATED_BODY()

public:
	float UseTime;
};
