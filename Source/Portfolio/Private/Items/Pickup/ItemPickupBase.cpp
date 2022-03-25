// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Pickup/ItemPickupBase.h"

#include "PortfolioCharacter.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogItemPickupBase, All, All);

AItemPickupBase::AItemPickupBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneComponent);
	//StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI"));
	UI->SetupAttachment(StaticMesh);

	OutLine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OutLine"));
	OutLine->SetupAttachment(StaticMesh);
	OutLine->SetCollisionProfileName(TEXT("NoCollision"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(SceneComponent);
	Box->SetBoxExtent(FVector(220.0f, 220.0f, 100.0f));
	Box->SetCollisionResponseToChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Box->OnComponentBeginOverlap.AddDynamic(this, &AItemPickupBase::OnBoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AItemPickupBase::OnOverlapEnd);
}

void AItemPickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	FVector ActorLocation = GetActorLocation();
	float ActorLocationZ = ActorLocation.Z * 10.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> Land = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	ObjectTypes.Add(Land);
	FHitResult Outhit;
	UKismetSystemLibrary::LineTraceSingleForObjects(this, ActorLocation, FVector(ActorLocation.X, ActorLocation.Y, ActorLocationZ),
	                                                ObjectTypes, false, {},
	                                                EDrawDebugTrace::ForDuration, Outhit, false, FLinearColor::Red,
	                                                FLinearColor::Green, 500.0f);
	
	SetActorLocation(Outhit.Location, false, nullptr, ETeleportType::None);

	if (GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}
}

void AItemPickupBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	FOnOverlepBox.Unbind();
}

void AItemPickupBase::InitPickup(EItemType ItemTypeRef, FText NameRef, FText UIPrefixRef, UStaticMesh* StaticMeshRef)
{
	Init(ItemTypeRef, NameRef);
	UIPrefix = UIPrefixRef;

	StaticMesh->SetStaticMesh(StaticMeshRef);
}

void AItemPickupBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if(OtherActor)
		{
			if (FOnOverlepBox.IsBound())
			{
				FOnOverlepBox.Execute(this, OtherActor);
			}
		
			if(OtherActor)
			{
				if (APortfolioCharacter* Player = Cast<APortfolioCharacter>(OtherActor))
				{
					Player->InventoryComponent->AddPickupItems(this);
				}
			}
			EnabledOutLine(true);
		}
	}
}
void AItemPickupBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (FEndOverlapBox.IsBound())
		{
			FEndOverlapBox.Execute(this);
		}
		
		if(OtherActor)
		{
			if (APortfolioCharacter* Player = Cast<APortfolioCharacter>(OtherActor))
			{
				Player->InventoryComponent->SubPickupItems(this);
			}
		}
		EnabledOutLine(false);
	}
}

void AItemPickupBase::EnabledOutLine(bool bEnabled)
{
	MulticastEnabledOutLine(bEnabled);
}


void AItemPickupBase::MulticastEnabledOutLine_Implementation(bool bEnabled)
{
	if (bEnabled)
	{
		StaticMesh->SetRenderCustomDepth(true);
	}
	else
	{
		StaticMesh->SetRenderCustomDepth(false);
	}
}

bool AItemPickupBase::MulticastEnabledOutLine_Validate(bool bEnabled)
{
	return true;
}
