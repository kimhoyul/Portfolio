// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnTest.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ASpawnTest::ASpawnTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASpawnTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

