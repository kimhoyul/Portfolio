// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "Items/ItemBase.h"
#include "EquipmentsWeapon.generated.h"

class AEquipmentsWeaponAcc;

UCLASS()
class PORTFOLIO_API AEquipmentsWeapon : public AItemBase
{
	GENERATED_BODY()
	
	AEquipmentsWeapon();

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateMag(AEquipmentsWeaponAcc* MagAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateMuzzle(AEquipmentsWeaponAcc* MuzzleAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateSight(AEquipmentsWeaponAcc* SightAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateForegrip(AEquipmentsWeaponAcc* ForegripAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateButtstock(AEquipmentsWeaponAcc* ButtstockAccActor);
	
private:
	UFUNCTION(BlueprintCallable)
	void PlayFiringSound();

	UFUNCTION(BlueprintCallable)
	void PlayFireFlash();

public:
	FItemWeapon* Datas;

	UPROPERTY()
	AEquipmentsWeaponAcc* AccMagObj;
	
	UPROPERTY()
	AEquipmentsWeaponAcc* AccMuzzleObj;

	UPROPERTY()
	AEquipmentsWeaponAcc* AccSightObj;

	UPROPERTY()
	AEquipmentsWeaponAcc* AccForegripObj;
	
	UPROPERTY()
	AEquipmentsWeaponAcc* AccButtstockObj;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	EWeaponPosition Position;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	int32 Ammo;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	bool bIsOnHand;

private:
	void InitDefaultAcc();
	
	USkeletalMeshComponent* SkeletalMesh;
	UStaticMeshComponent* Mag;
	UStaticMeshComponent* Muzzle;
	UStaticMeshComponent* Sight;
	UStaticMeshComponent* Foregrip;
	UStaticMeshComponent* Buttstock;
	UAudioComponent* Audio;
	UParticleSystemComponent* FireFlash;
	
	UPROPERTY()
	UDataTable* ItemWeaponTable;

	UPROPERTY()
	UDataTable* ItemWeaponAccTable;
};
