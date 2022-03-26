// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "ItemPickupBase.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class PORTFOLIO_API AItemPickupBase : public AItemBase
{
	GENERATED_BODY()

public:
	AItemPickupBase();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InInventory(bool In);
	
	UPROPERTY(ReplicatedUsing = OnRep_Pickup)
	bool ObjectPickup;

	UFUNCTION()
	void OnRep_Pickup();
	
	/////////////////////////////////////////////////////////////////////////
	// Components
	UBoxComponent* GetIBoxComponent() { return Box; }

	UStaticMeshComponent* GetIStaticMeshComponent() { return StaticMesh; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* UI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OutLine", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* OutLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;
	
public:
	/////////////////////////////////////////////////////////////////////////
	// Shining Out Line | Trigger
private:
	UFUNCTION()
	 void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/////////////////////////////////////////////////////////////////////////
	// Shining Out Line | Delegate
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnOverlepBoxSignature, AItemPickupBase*, PickupObj, AActor*, OtherActor);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FEndOverlepBoxSignature, AItemPickupBase*, PickupObj);
	
	UPROPERTY()
	FOnOverlepBoxSignature FOnOverlepBox;
	FEndOverlepBoxSignature FEndOverlapBox;
	
protected:
	/////////////////////////////////////////////////////////////////////////
	// Set property
	virtual void InitPickup(EItemType ItemTypeRef, FText NameRef, FText UIPrefixRef, UStaticMesh* StaticMeshRef);
	
	FText UIPrefix;

	// /////////////////////////////////////////////////////////////////////////
	// // Shining Out Line
	// UFUNCTION()
	// void EnabledOutLine(bool bEnabled);
	//
	// UFUNCTION(NetMulticast, Reliable, WithValidation)
	// void MulticastEnabledOutLine(bool bEnabled);
};

