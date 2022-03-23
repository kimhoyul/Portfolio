// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PortfolioPlayerState.h"

#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void APortfolioPlayerState::SetWeapon1(AEquipmentsWeapon* Weapon)
{
	Weapon1 = Weapon;
	OnWeaponChanged.Broadcast(Weapon1, EWeaponPosition::EWP_Left, false);
}

void APortfolioPlayerState::SetWeapon2(AEquipmentsWeapon* Weapon)
{
	Weapon2 = Weapon;
	OnWeaponChanged.Broadcast(Weapon1, EWeaponPosition::EWP_Right, false);
}

void APortfolioPlayerState::SetHoldGun(AEquipmentsWeapon* Weapon)
{
	HoldGun = Weapon;
	if (IsValid(HoldGun))
	{
		OnWeaponChanged.Broadcast(HoldGun, HoldGun->Position, true);
	}
	else
	{
		OnWeaponChanged.Broadcast(HoldGun, EWeaponPosition::EWP_Left, true);
	}
}

void APortfolioPlayerState::SetAmmo556(int32 Value)
{
	Ammo556 = Value;
	OnAmmoChanged.Broadcast();
}

void APortfolioPlayerState::SetAmmo762(int32 Value)
{
	Ammo762 = Value;
	OnAmmoChanged.Broadcast();
}

void APortfolioPlayerState::SetHealthPoint(float Value)
{
	HealthPoint = Value;
	OnHealthPointChanged.Broadcast(HealthPoint);
}

void APortfolioPlayerState::SetEnergyPoint(float Value)
{
	EnergyPoint = Value;
	OnEnergyPointChanged.Broadcast(EnergyPoint);
}

void APortfolioPlayerState::AddEquipment(AItemBase* EquipmentsRef)
{
	Equipments.Add(EquipmentsRef);
	OnEquipmentsChanged.Broadcast(EquipmentsRef, true);
}

void APortfolioPlayerState::AddClothes(AItemBase* ClothesRef)
{
	Clothes.Add(ClothesRef);
	OnClothesChanged.Broadcast(ClothesRef, true);
}

//////////////////////////////////////////////////////////////////////////
// If the ItemType is a Health or Boost, if there is already an item of the same type, the quantity is added directly
void APortfolioPlayerState::AddItem(AItemBase* Item)
{
	bool IsFound = false;

	if (Item->ItemType == EItemType::EIT_Health || Item->ItemType == EItemType::EIT_Boost)
	{
		for (AItemBase* ArrayElement : ItemsInBackpack)
		{
			bool A = ArrayElement->ItemType == Item->ItemType;
			bool B = Item->ID == ArrayElement->ID;

			if (A && B)
			{
				ArrayElement->Amount = ArrayElement->Amount + Item->Amount;
				IsFound = true;
				OnItemsChanged.Broadcast(Item, true);
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Not a Health or Boost, or no identical item, is added to the item object 
		if (!IsFound)
		{
			ItemsInBackpack.Add(Item);
			OnItemsChanged.Broadcast(Item, true);
		}
	}
	else
	{
		if (!IsFound)
		{
			ItemsInBackpack.Add(Item);
			OnItemsChanged.Broadcast(Item, true);
		}
	}
}

void APortfolioPlayerState::RemoveEquipment(AItemBase* EquipmentsRef, bool& Removed)
{
	if (Equipments.Find(EquipmentsRef) != -1)
	{
		Equipments.RemoveSingle(EquipmentsRef);
		OnEquipmentsChanged.Broadcast(EquipmentsRef, false);
		Removed = true;
	}
	else if (Equipments.Find(EquipmentsRef) == -1)
	{
		OnEquipmentsChanged.Broadcast(EquipmentsRef, false);
		Removed = false;
	}
}

void APortfolioPlayerState::RemoveClothes(AItemBase* ClothesRef, bool& Removed)
{
	if (Clothes.Find(ClothesRef) != -1)
	{
		Clothes.RemoveSingle(ClothesRef);
		OnClothesChanged.Broadcast(ClothesRef, false);
		Removed = true;
	}
	else if (Clothes.Find(ClothesRef) == -1)
	{
		OnClothesChanged.Broadcast(ClothesRef, false);
		Removed = false;
	}
}

void APortfolioPlayerState::RemoveItem(AItemBase* ItemRef, bool& Removed)
{
	if (ItemsInBackpack.Find(ItemRef) != -1)
	{
		ItemsInBackpack.RemoveSingle(ItemRef);
		OnItemsChanged.Broadcast(ItemRef, false);
		Removed = true;
	}
	else if (ItemsInBackpack.Find(ItemRef) == -1)
	{
		OnItemsChanged.Broadcast(ItemRef, false);
		Removed = false;
	}
}
