// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Items/ItemBase.h"
#include "ItemPickupBase.generated.h"

UCLASS()
class PORTFOLIO_API AItemPickupBase : public AItemBase
{
	GENERATED_BODY()

public:
	AItemPickupBase();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 

	UFUNCTION()
	void EnabledOutLine(bool bEnabled);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastEnabledOutLine(bool bEnabled);
	
	FText UIPrefix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;
	
	UPROPERTY()
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY()
	UWidgetComponent* UI;
	UPROPERTY()
	UStaticMeshComponent* OutLine;
	
protected:
	virtual void PostInitializeComponents() override; 
	virtual void InitPickup(EItemType ItemTypeRef, FText NameRef, FText UIPrefixRef, UStaticMesh* StaticMeshRef);

private:
	UFUNCTION()
 	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnOverlepBoxSignature, AItemPickupBase*, PickupObj, AActor*, OtherActor);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FEndOverlepBoxSignature, AItemPickupBase*, PickupObj);
	
	UPROPERTY()
	FOnOverlepBoxSignature FOnOverlepBox;
	FEndOverlepBoxSignature FEndOverlapBox;

	UFUNCTION()
	void OnOverlap(AItemPickupBase* Pickupobj);

	UFUNCTION()
	void EndOverlap(AItemPickupBase* Pickupobj);
};

