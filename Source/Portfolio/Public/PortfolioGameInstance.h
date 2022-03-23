// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PortfolioGameInstance.generated.h"

UCLASS()
class PORTFOLIO_API UPortfolioGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPortfolioGameInstance();
	
	FString GenerateSN();

	FName IDBackup;
	
	//////////////////////////////////////////////////////////////////////////
	// BackPack
	int32 DefaultCapacity;

	//////////////////////////////////////////////////////////////////////////
	// SocketName | Weapon
	FName GunStandName;
	FName GunAimName;
	FName GunReloadName;
	FName GunProneIdleName;
	FName GunProneFBName;
	FName GunProneOtherName;
	FName GunCrouchName;
	FName BackLeftNName;
	FName BackRightNName;
	FName BackLeftBName;
	FName BackRightBName;
	
	//////////////////////////////////////////////////////////////////////////
	// SocketName | Equipment

	FName HelmetName;
	FName VestName;
	FName BackpackName;
};
