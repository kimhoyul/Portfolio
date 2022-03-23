// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EnumData.h"
#include "Engine/DataTable.h"
#include "StructData.generated.h"

USTRUCT(BlueprintType)
struct FItemClothes : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClothesType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* MaskTexture = nullptr;
};

USTRUCT(BlueprintType)
struct FItemWeapon : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon= nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BigIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UseAmmoID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClipSize = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FiringInterval = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReplaceClipTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReplaceBulletTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Automode = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* DefaultMag = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* DefaultSight = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* DefaultButtstock = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FireSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FireSoundSuppressor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanSight = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMuzzle = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanClip = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanButtstock = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanGrip = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMin_Stillness = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMAX_Stillness = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMin_Move = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMax_Move = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMin_AimStillness = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMax_AimStillness = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMin_AimMove = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMax_AimMove = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalOffset = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalOffset = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitVelocity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Velocity100M = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Velocity300M = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Velocity500M = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeedPer = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent = 0;
};

USTRUCT(BlueprintType)
struct FItemWeaponAcc : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponAccType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> WeaponIDs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SightMultiple = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSilencer = false; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsNotFlash = false; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClipRate = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClipCapacity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalAdjPer = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalAdjPer = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};

USTRUCT(BlueprintType)
struct FItemHealth : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UseTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Limit = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent = 0;
	
};

USTRUCT(BlueprintType)
struct FItemBoost : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UseTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoostValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent = 0;
};

USTRUCT(BlueprintType)
struct FItemAmmo : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* PickupMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PickupAmmo = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent = 0;
};

USTRUCT(BlueprintType)
struct FItemEquipment : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail  = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Capacity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent = 0;
};

USTRUCT(BlueprintType)
struct FNumberProbability : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Percent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Number = 0;
};

USTRUCT(BlueprintType)
struct FItemTypeProbability : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Percent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type;
};

USTRUCT(BlueprintType)
struct FItemGroupLocation : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FItemTypeAndID : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;
};