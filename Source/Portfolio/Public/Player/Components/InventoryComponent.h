// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddPickupItemList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSubPickupItemList);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	void AddPickupItems(class AItemPickupBase* Items);
	void SubPickupItems(class AItemPickupBase* Items);
	void DiscardWeapon(class AEquipmentsWeapon* ItemWeapon);
	void DiscardItem(class AItemBase* Item);
	bool RemoveItem(AItemBase* ItemRef);

	UFUNCTION(BlueprintCallable)
	TArray<AItemPickupBase*> GetItemsInRange() { return ItemsInRange; }

	UFUNCTION(BlueprintCallable)
	TArray<AItemPickupBase*> GetBackpackItem() { return ItemsInBackpack; }

	UPROPERTY(BlueprintReadWrite)
	int32 WeaponSlot;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* EquipmentHelmet;
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentHelmetLevel;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* EquipmentVest;
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentVestLevel;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* EquipmentBackpack;
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentBackpackLevel;
	
	UPROPERTY(BlueprintReadWrite)
	UTexture2D* EquipmentClothTop;
	
	UPROPERTY(BlueprintReadWrite)
	UTexture2D* EquipmentClothBottom;
	
	UPROPERTY(BlueprintReadWrite)
	UTexture2D* EquipmentClothShoes;
	
	UPROPERTY(BlueprintReadWrite)
	AItemBase* BaseWeapon1;

	UPROPERTY(BlueprintReadWrite)
	AItemBase* BaseWeapon2;

	UPROPERTY(BlueprintReadWrite)
	bool HasWeapon1;

	UPROPERTY(BlueprintReadWrite)
	bool HasWeapon2;
	
	UFUNCTION(BlueprintCallable)
	bool AddBackpackItem(AItemPickupBase* PointItem);
	
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<class AItemPickupBase*> ItemsInRange;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<class AItemPickupBase*> ItemsInBackpack;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 BackpackCapacity;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 TotalWeight;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 Ammo556;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 Ammo762;

	
public:
	UPROPERTY(BlueprintAssignable)
	FOnAddPickupItemList AddPickupItemList;

	UPROPERTY(BlueprintAssignable)
	FOnSubPickupItemList SubPickupItemList;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAddPickupItems(class AItemPickupBase* Items);
	
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSubPickupItems(class AItemPickupBase* Items);

private:
	UPROPERTY()
	UDataTable* ItemWeaponTable;

	UPROPERTY()
	UDataTable* ItemWeaponAccTable;

	UPROPERTY()
	UDataTable* ItemAmmoTable;

	UPROPERTY()
	UDataTable* ItemHealthTable;

	UPROPERTY()
	UDataTable* ItemBoostTable;

	UPROPERTY()
	UDataTable* ItemEquipmentsTable;

	UPROPERTY()
	UDataTable* ItemClothesTable;
};
