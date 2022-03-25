// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "PortfolioCharacter.h"
#include "PortfolioPlayerController.h"
#include "Net/UnrealNetwork.h"

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	PortfolioCharacter = Cast<APortfolioCharacter>(GetOwner());
	if (!PortfolioCharacter) return;
	PortfolioPlayerController = Cast<APortfolioPlayerController>(PortfolioCharacter->GetController());
	if (!PortfolioPlayerController) return;

	if (GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}
	
}

void AItemBase::Init(EItemType ItemTypeRef, FText NameRef)
{
	ItemType = ItemTypeRef;
	Name = NameRef;
}

void AItemBase::GetWeight(int32 &Weight)
{
}

void AItemBase::UpdateAmount(bool bAdd, bool bReduce, int32 AmountRef)
{
	if (bAdd)
	{
		Amount = Amount + AmountRef;
	}
	else
	{
		if (bReduce)
		{
			Amount = Amount - AmountRef;
		}
		else
		{
			Amount = AmountRef;
		}
	}
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ID);
	DOREPLIFETIME(AItemBase, SN);
	DOREPLIFETIME(AItemBase, Amount);
}