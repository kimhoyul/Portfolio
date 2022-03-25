// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/SpawnPoint.h"
#include "Components/StaticMeshComponent.h"

ASpawnPoint::ASpawnPoint()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	AActor::SetActorHiddenInGame(true);
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

