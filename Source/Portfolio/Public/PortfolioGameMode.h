// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "GameFramework/GameModeBase.h"
#include "PortfolioGameMode.generated.h"

class APortfolioPlayerController;
class AItemPickupBase;
class UPortfolioGameInstance;

UCLASS(minimalapi)
class APortfolioGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APortfolioGameMode();

	virtual void BeginPlay() override;

	void Respawn(APortfolioPlayerController* PC);

	FVector GetSpawnPoint();

protected:
	//////////////////////////////////////////////////////////////////////////
	// spawn
	
	UFUNCTION()
	void Spawn(APortfolioPlayerController* Controller);

	FVector DefaultSpawnLocation;

	UPROPERTY(EditAnywhere, Category = "Respawn")
	float RespawnTime;
		
	UPROPERTY()
	TArray<FVector> SpawnPoints;

	//////////////////////////////////////////////////////////////////////////
	// spawn | DataTable variables
	UPROPERTY()
	UDataTable* SpawnPointLocationTable;

private:
	//////////////////////////////////////////////////////////////////////////
	// Generate Random Items
	UFUNCTION()
	void GenerateItems(TArray<AItemPickupBase*>& Items);

	UFUNCTION()
	void RandomItemNumber(int32 &Number);

	UFUNCTION()
	void RandomItemType(EItemType &ItemType);

	UFUNCTION()
	void RandomItemID(EItemType ItemTypeRef, FItemTypeAndID& ItemRef);

	UFUNCTION()
	void RandomLocation(TArray<FTransform> Available, FTransform& Location);

	void ShuffleArray(TArray<int32>& IntArr, TArray<EItemType> &EnumArr, TArray<FItemTypeAndID>& StrutRef);
	
	//////////////////////////////////////////////////////////////////////////
	// Generate Random Items | Structure variables
	FNumberProbability* NumberProbabilityData;

	FItemTypeProbability* ItemTypeProbabilityData;

	FItemWeapon* ItemWeaponData;

	FItemWeaponAcc* ItemWeaponAccData;

	FItemAmmo* ItemAmmoData;

	FItemHealth* ItemHealthData;

	FItemBoost* ItemBoostData;

	FItemEquipment* ItemEquipmentData;

	FItemClothes* temClothesData;

	//////////////////////////////////////////////////////////////////////////
	// Generate Random Items | DataTable variables
	UPROPERTY()
	UDataTable* ItemGroupLocationTable;

	UPROPERTY()
	UDataTable* NumberProbabilityTable;

	UPROPERTY()
	UDataTable* ItemTypeProbabilityTable;

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
	
	//////////////////////////////////////////////////////////////////////////
	// Enum To String
	FString GetEItemTypeAsString(EItemType EnumValue);

	//////////////////////////////////////////////////////////////////////////
	// GameFrameWork Ref
	UPROPERTY()
	UPortfolioGameInstance* GameInstanceRef;

	UPROPERTY()
	APortfolioPlayerController* Controller;
};


