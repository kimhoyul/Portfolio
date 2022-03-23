// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemGroup.h"

AItemGroup::AItemGroup()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	StaticMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh1"));
	StaticMesh1->SetupAttachment(SceneComponent);
	StaticMesh1->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	StaticMesh1->SetCollisionProfileName(TEXT("NoCollision"));

	StaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh2"));
	StaticMesh2->SetupAttachment(SceneComponent);
	StaticMesh2->SetRelativeLocation(FVector(-80.0f, 60.0f, 0.0f));
	StaticMesh2->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	StaticMesh2->SetCollisionProfileName(TEXT("NoCollision"));

	StaticMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh3"));
	StaticMesh3->SetupAttachment(SceneComponent);
	StaticMesh3->SetRelativeLocation(FVector(-30.0f, -80.0f, 0.0f));
	StaticMesh3->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	StaticMesh3->SetCollisionProfileName(TEXT("NoCollision"));

	StaticMesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh4"));
	StaticMesh4->SetupAttachment(SceneComponent);
	StaticMesh4->SetRelativeLocation(FVector(40.0f, 140.0f, 0.0f));
	StaticMesh4->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	StaticMesh4->SetCollisionProfileName(TEXT("NoCollision"));

	StaticMesh5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh5"));
	StaticMesh5->SetupAttachment(SceneComponent);
	StaticMesh5->SetCollisionProfileName(TEXT("NoCollision"));

	bReplicates = true;
}

void AItemGroup::GetAllTransform(TArray<FTransform> &Transform)
{
	Transform.Add(StaticMesh1->GetComponentToWorld());
	Transform.Add(StaticMesh2->GetComponentToWorld());
	Transform.Add(StaticMesh3->GetComponentToWorld());
	Transform.Add(StaticMesh4->GetComponentToWorld());
	Transform.Add(StaticMesh5->GetComponentToWorld());
}
