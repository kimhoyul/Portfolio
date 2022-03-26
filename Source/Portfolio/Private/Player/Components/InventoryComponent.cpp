// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Items/Pickup/ItemPickupBase.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	/////////////////////////////////////////////////////////////////////////
	// Class Replicates
	if (GetOwnerRole() == ROLE_Authority)
	{
		SetIsReplicated(true);
	}
}
	
/////////////////////////////////////////////////////////////////////////
// Add items from inventory
bool UInventoryComponent::AddItems(AItemPickupBase* Item)
{
	ItemsInBackpack.Add(Item);
	Item->InInventory(true);
	
	for (AItemPickupBase* ArrayElement : ItemsInBackpack)
	{
		GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow,FString::Printf(TEXT("InvetoryComponent : [%s]"), *ArrayElement->GetName()));
	}
	
	return false;
}

/////////////////////////////////////////////////////////////////////////
// Drop All items from inventory
void UInventoryComponent::DropAllInventory()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (AItemPickupBase* ArrayElement : ItemsInBackpack)
		{
			DropItems(ArrayElement);
		}
		ItemsInBackpack.Empty();
	}
}

/////////////////////////////////////////////////////////////////////////
// Drop items from inventory
void UInventoryComponent::DropItems(AItemPickupBase* Item)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		FVector Location = GetOwner()->GetActorLocation();
		Location.X += FMath::RandRange(-50.0f, 100.0f);
		Location.Y += FMath::RandRange(-50.0f, 100.0f);

		FVector EndRay = Location;
		EndRay.Z -= 500.0f;
		
		FHitResult HitResult;
		FCollisionObjectQueryParams CollisionParams;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());
		GetWorld()->LineTraceSingleByObjectType(
			OUT HitResult,
			Location,
			EndRay,
			CollisionParams,
			CollisionQueryParams
			);

		if (HitResult.ImpactPoint != FVector::ZeroVector)
		{
			Location = HitResult.ImpactPoint;
		}
		
		Item->SetActorLocation(Location);
		Item->InInventory(false);
	}
}

/////////////////////////////////////////////////////////////////////////
// variables Replicates
void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UInventoryComponent, ItemsInBackpack, COND_OwnerOnly);
}