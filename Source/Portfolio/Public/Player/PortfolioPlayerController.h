// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Items/Pickup/ItemPickupBase.h"
#include "PortfolioPlayerController.generated.h"

class APortfolioPlayerState;

UCLASS()
class PORTFOLIO_API APortfolioPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APortfolioPlayerController();
	
	virtual void OnRep_PlayerState() override;
	
	virtual void BeginPlay() override;
	
	bool IsGameInputAllowed() const;
	
	UFUNCTION()
	void SetPickupItems(TArray<AItemPickupBase*> Items);

	UFUNCTION()
	void ExecBeginOverlap(AItemPickupBase* PickupObject, AActor* OtherActor);

	UFUNCTION()
	void ExecEndOverlap(AItemPickupBase* PickupObject);
	
	UFUNCTION()
	void OutlineIte(AItemPickupBase* Item);
	
	UFUNCTION(BlueprintCallable)
    void AutoPosition(EWeaponPosition& Position, bool& IsOnHand);

	UFUNCTION(BlueprintCallable)
	void AssignPosition(EWeaponPosition Assign, EWeaponPosition& Position, bool& IsOnHand);

	UFUNCTION(BlueprintCallable)
	void SpawnPickupItem(AItemBase* ItemBase, AItemPickupBase*& PickupItem);
	
	UPROPERTY()
	AItemPickupBase* ReadyPickupItem;

protected:
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	
	UPROPERTY()
	TArray<AItemPickupBase*> PickupItems;

	UPROPERTY(Replicated)
	TArray<AItemPickupBase*> ItemsInRange;

	UPROPERTY()
	APortfolioCharacter* PortfolioCharacterRef;

	UPROPERTY()
	APortfolioPlayerState* PortfolioPlayerStateRef;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerOnTest(AItemPickupBase* PickupObject);

};
