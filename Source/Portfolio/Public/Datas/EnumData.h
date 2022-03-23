// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnumData.generated.h"

UENUM(BlueprintType)
enum class EClothesType : uint8
{
	ECT_ClothTop UMETA(DisplayName = "ClothTop"),
	ECT_ClothBottom UMETA(DisplayName = "ClothBottom"),
	ECT_Whole UMETA(DisplayName = "Whole"),
	ECT_Shoes UMETA(DisplayName = "Shoes")
};

UENUM(BlueprintType)
enum class EWeaponAccType : uint8
{
	EWAT_Sight UMETA(DisplayName = "Sight"),
	EWAT_Muzzle UMETA(DisplayName = "Muzzle"),
	EWAT_Foregrip UMETA(DisplayName = "Foregrip"),
	EWAT_Mag UMETA(DisplayName = "Mag"),
	EWAT_Buttstock UMETA(DisplayName = "Buttstock")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Accesories UMETA(DisplayName = "Accesories"),
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Health UMETA(DisplayName = "Health"),
	EIT_Boost UMETA(DisplayName = "Boost"),
	EIT_Helmet UMETA(DisplayName = "Helmet"),
	EIT_Vest UMETA(DisplayName = "Vest"),
	EIT_Backpack UMETA(DisplayName = "Backpack"),
	EIT_Clothes UMETA(DisplayName = "Clothes")
};

UENUM(BlueprintType)
enum class EWeaponPosition : uint8
{
	EWP_Left UMETA(DisplayName = "Left"),
	EWP_Right UMETA(DisplayName = "Right")
};