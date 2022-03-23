// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Datas/StructData.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnTest.h"
#include "PortfolioGameMode.generated.h"

class AItemPickupBase;
class APortfolioPlayerController;

class UPortfolioGameInstance;

UCLASS(minimalapi)
class APortfolioGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APortfolioGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFramework")
	APortfolioPlayerController* PlayerController;

	UFUNCTION(Category = "Generat Items")
	void GenerateItems(TArray<AItemPickupBase*>& Items);

	UFUNCTION(Category = "Generat Items")
	void RandomItemNumber(int32 &Number);

	UFUNCTION(Category = "Generat Items")
	void RandomItemType(EItemType &ItemType);

	UFUNCTION(Category = "Generat Items")
	void RandomItemID(EItemType ItemTypeRef, FItemTypeAndID& ItemRef);

	UFUNCTION(Category = "Generat Items")
	void RandomLocation(TArray<FTransform> Available, FTransform& Location);

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpawnTest> SpawnClass;
	
private:

	void ShuffleArray(TArray<int32>& IntArr, TArray<EItemType> &EnumArr, TArray<FItemTypeAndID>& StrutRef);
	
	FNumberProbability* NumperProbabilityDatas;

	FItemTypeProbability* ItemTypeProbabilityDatas;

	FItemWeapon* ItemWeaponDatas;

	FItemWeaponAcc* ItemWeaponAccDatas;

	FItemAmmo* ItemAmmoDatas;

	FItemHealth* ItemHealthDatas;

	FItemBoost* ItemBoostDatas;

	FItemEquipment* ItemEquipmentDatas;

	FItemClothes* temClothesDatas;
	
	UPROPERTY()
	UPortfolioGameInstance* GameInstanceRef;
	
	UPROPERTY()
	UDataTable* ItemGroupLocationTable;

	UPROPERTY()
	UDataTable* NumperProbabilityTable;

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

	FString GetEItemTypeAsString(EItemType EnumValue);

	UFUNCTION()
	void SpawnTest();
};


