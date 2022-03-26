// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItemPickupBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	//////////////////////////////////////////////////////////////////////////
	// Add or remove items from inventory
	bool AddItems(AItemPickupBase* Item);
	
	void DropAllInventory();
	
	void DropItems(AItemPickupBase* Item);


public:	
	//////////////////////////////////////////////////////////////////////////
	// Character Inventory
	UFUNCTION(BlueprintCallable)
	TArray<AItemPickupBase*> GetBackpackItem() { return ItemsInBackpack; }

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<AItemPickupBase*> ItemsInBackpack;

};
