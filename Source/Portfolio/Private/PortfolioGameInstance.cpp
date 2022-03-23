// Fill out your copyright notice in the Description page of Project Settings.


#include "PortfolioGameInstance.h"
#include "Kismet/KismetMathLibrary.h"

UPortfolioGameInstance::UPortfolioGameInstance()
{
	//////////////////////////////////////////////////////////////////////////
	// BackPack
	DefaultCapacity = 50;

	//////////////////////////////////////////////////////////////////////////
	// SocketName | Weapon
	GunStandName = "Socket_Gun_Stand";
	GunAimName = "Socket_Gun_Aim";
	GunReloadName  = "Socket_Gun_Reload";
	GunProneIdleName  = "Socket_Gun_ProneIdle";
	GunProneFBName  = "Socket_Gun_ProneFB";
	GunProneOtherName  = "Socket_Gun_ProneOther";
	GunCrouchName  = "Socket_Gun_Crouch";
	BackLeftNName  = "Socket_BackLeft_Normal";
	BackRightNName  = "Socket_BackRight_Normal";
	BackLeftBName  = "Socket_BackLeft_Backpack";
	BackRightBName  = "Socket_BackRight_Backpack";
	
	//////////////////////////////////////////////////////////////////////////
	// SocketName | Equipment

	HelmetName = "Socket_Helmet";
	VestName = "Socket_Vest";
	BackpackName = "Socket_Backpack";
}

FString UPortfolioGameInstance::GenerateSN()
{
	float RandomFloat1 = UKismetMathLibrary::RandomFloat();
	float RandomFloat2 = UKismetMathLibrary::RandomFloat();
	FString SN = FString::SanitizeFloat(RandomFloat1) + FString::SanitizeFloat(RandomFloat2);
	return SN;
}
