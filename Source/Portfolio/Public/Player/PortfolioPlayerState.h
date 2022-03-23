// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Items/Equipments/EquipmentsWeapon.h"
#include "PortfolioPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FOnWeaponChanged, AEquipmentsWeapon*, Weapon, EWeaponPosition, Position, bool, IsOnHold );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnAmmoChanged );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnHealthPointChanged, float, Heath );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnEnergyPointChanged, float, Energy );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnEquipmentsChanged, AItemBase*, Equipment, bool, IsAdd );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnClothesChanged, AItemBase*, Clothes, bool, IsAdd );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnItemsChanged, AItemBase*, Item, bool, IsAddl );

class AItemPickupBase;

UCLASS()
class PORTFOLIO_API APortfolioPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	//////////////////////////////////////////////////////////////////////////
	// Getter
	UFUNCTION(BlueprintCallable)
	AEquipmentsWeapon* GetWeapon1() const { return Weapon1; }

	UFUNCTION(BlueprintCallable)
	AEquipmentsWeapon* GetWeapon2() const { return Weapon2; }

	UFUNCTION(BlueprintCallable)
	AEquipmentsWeapon* GetHoldGun() const { return HoldGun; }

	int32 GetAmmo556() const { return Ammo556; }

	int32 GetAmmo762() const { return Ammo762; }

	float GetHealthPoint() const { return HealthPoint; }

	float GetEnergyPoint() const { return EnergyPoint; }

	TArray<AItemBase*> GetEquipments() const { return Equipments; }
	
	TArray<AItemBase*> GetClothes() const { return Clothes; }

	TArray<AItemBase*> GetItems() const { return ItemsInBackpack; }

	//////////////////////////////////////////////////////////////////////////
	// Setter
	UFUNCTION(BlueprintCallable)
	void SetWeapon1(AEquipmentsWeapon* Weapon);

	UFUNCTION(BlueprintCallable)
	void SetWeapon2(AEquipmentsWeapon* Weapon);

	UFUNCTION(BlueprintCallable)
	void SetHoldGun(AEquipmentsWeapon* Weapon);

	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

	UFUNCTION(BlueprintCallable)
	void SetAmmo556(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetAmmo762(int32 Value);

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged OnAmmoChanged;

	UFUNCTION(BlueprintCallable)
	void SetHealthPoint(float Value);

	UPROPERTY(BlueprintAssignable)
	FOnHealthPointChanged OnHealthPointChanged;

	UFUNCTION(BlueprintCallable)
	void SetEnergyPoint(float Value);

	UPROPERTY(BlueprintAssignable)
	FOnEnergyPointChanged OnEnergyPointChanged;

	//////////////////////////////////////////////////////////////////////////
	// Array Add
	UFUNCTION(BlueprintCallable)
	void AddEquipment(AItemBase* EquipmentsRef);

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentsChanged OnEquipmentsChanged;

	UFUNCTION(BlueprintCallable)
	void AddClothes(AItemBase* ClothesRef);

	UPROPERTY(BlueprintAssignable)
	FOnClothesChanged OnClothesChanged;
	
	UFUNCTION(BlueprintCallable)
	void AddItem(AItemBase* Item);

	UPROPERTY(BlueprintAssignable)
	FOnItemsChanged OnItemsChanged;

	//////////////////////////////////////////////////////////////////////////
	// Array Remove
	UFUNCTION(BlueprintCallable)
	void RemoveEquipment(AItemBase* EquipmentsRef, bool& Removed);

	UFUNCTION(BlueprintCallable)
	void RemoveClothes(AItemBase* EquipmentsRef, bool& Removed);

	void RemoveItem(AItemBase* ItemRef, bool& Removed);
	
private:
	UPROPERTY()
	AEquipmentsWeapon* Weapon1;

	UPROPERTY()
	AEquipmentsWeapon* Weapon2;

	UPROPERTY()
	AEquipmentsWeapon* HoldGun;

	UPROPERTY()
	TArray<AItemBase*> Equipments;

	UPROPERTY()
	TArray<AItemBase*> Clothes;

	UPROPERTY()
	TArray<AItemBase*> ItemsInBackpack;
	
	int32 Ammo556;

	int32 Ammo762;

	int32 KilledAmount;
	
	float HealthPoint = 100;
	
	float EnergyPoint = 100;

public: 
	UPROPERTY()
	TArray<AItemPickupBase*> ItemsInRange;
};
