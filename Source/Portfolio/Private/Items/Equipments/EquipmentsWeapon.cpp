// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipments/EquipmentsWeapon.h"
#include "Components/AudioComponent.h"
#include "Items/Equipments/EquipmentsWeaponAcc.h"
#include "Particles/ParticleSystemComponent.h"

AEquipmentsWeapon::AEquipmentsWeapon()
{
	//////////////////////////////////////////////////////////////////////////
	// Attach Component
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
	
	Mag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mag"));
	Mag->SetupAttachment(SkeletalMesh);
	Mag->SetCollisionProfileName(TEXT("NoCollision"));

	Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(SkeletalMesh);
	Muzzle->SetCollisionProfileName(TEXT("NoCollision"));

	Sight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sight"));
	Sight->SetupAttachment(SkeletalMesh);
	Sight->SetCollisionProfileName(TEXT("NoCollision"));

	Foregrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foregrip"));
	Foregrip->SetupAttachment(SkeletalMesh);
	Foregrip->SetCollisionProfileName(TEXT("NoCollision"));

	Buttstock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Buttstock"));
	Buttstock->SetupAttachment(SkeletalMesh);
	Buttstock->SetCollisionProfileName(TEXT("NoCollision"));

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(SkeletalMesh);
	
	FireFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireFlash"));
	FireFlash->SetupAttachment(SkeletalMesh);
	FireFlash->SetAutoActivate(false);
	FireFlash->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleTemplate(TEXT("/Game/Assets/Particles/Particles/P_Weapon_Fired.P_Weapon_Fired"));
	if (ParticleTemplate.Succeeded())
	{
		FireFlash->SetTemplate(ParticleTemplate.Object);
	}
	// FireFlash->K2_AttachToComponent(SkeletalMesh, "Socket_Muzzle", EAttachmentRule::SnapToTarget,
	//                                 EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, true);
	FireFlash->SetupAttachment(SkeletalMesh, "Socket_Muzzle");
	//////////////////////////////////////////////////////////////////////////
	// Get data table
	
	const FString ItemWeaponDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_Weapon.DT_Item_Weapon");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_Weapon(*ItemWeaponDataPath);
	check(DT_Item_Weapon.Succeeded());
	ItemWeaponTable = DT_Item_Weapon.Object;
	check(ItemWeaponTable->GetRowMap().Num() > 0);

	const FString ItemWeaponAccDataPath = TEXT("/Game/Datas/DataTables/Items/DT_Item_WeaponAcc.DT_Item_WeaponAcc");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item_WeaponAcc(*ItemWeaponAccDataPath);
	check(DT_Item_WeaponAcc.Succeeded());
	ItemWeaponAccTable = DT_Item_WeaponAcc.Object;
	check(ItemWeaponAccTable->GetRowMap().Num() > 0);
}

void AEquipmentsWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Datas = ItemWeaponTable->FindRow<FItemWeapon>(ID,TEXT(""));
	Init(EItemType::EIT_Weapon, Datas->Name);

	SkeletalMesh->SetSkeletalMesh(Datas->SkeletalMesh, true);

	//////////////////////////////////////////////////////////////////////////
	// Default weapon accessory settings
	
	InitDefaultAcc();
}

void AEquipmentsWeapon::InitDefaultAcc()
{
	//////////////////////////////////////////////////////////////////////////
	// Attach Mag
	
	Mag->SetStaticMesh(Datas->DefaultMag);
	Mag->K2_AttachToComponent(SkeletalMesh, "Socket_Mag", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,
							  EAttachmentRule::KeepRelative, true);

	//////////////////////////////////////////////////////////////////////////
	// Attach Muzzle
	
	Muzzle->K2_AttachToComponent(SkeletalMesh, "Socket_Muzzle", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,
							  EAttachmentRule::KeepRelative, true);

	//////////////////////////////////////////////////////////////////////////
	// Attach Sight
	
	if (Datas->DefaultSight != nullptr)
	{
		Sight->SetStaticMesh(Datas->DefaultSight);
		Sight->K2_AttachToComponent(SkeletalMesh, "Socket_Sight_Default", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,
							  EAttachmentRule::KeepRelative, true);
	}

	//////////////////////////////////////////////////////////////////////////
	// Attach Foregrip
	
	if (Datas->CanGrip)
	{
		Foregrip->K2_AttachToComponent(SkeletalMesh, "Socket_Foregrip", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,
							  EAttachmentRule::KeepRelative, true);
	}

	//////////////////////////////////////////////////////////////////////////
	// Attach Buttstock
	
	if (Datas->DefaultButtstock != nullptr)
	{
		Buttstock->SetStaticMesh(Datas->DefaultButtstock);
		Buttstock->K2_AttachToComponent(SkeletalMesh, "Socket_Buttstock", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,
							  EAttachmentRule::KeepRelative, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// UpdateMag

void AEquipmentsWeapon::UpdateMag(AEquipmentsWeaponAcc* MagAccActor)
{
	AccMagObj = MagAccActor;
	if (AccMagObj == nullptr)
	{
		Mag->SetStaticMesh(Datas->DefaultMag); 
	}
	else
	{
		FItemWeaponAcc* AccData = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(AccMagObj->ID,TEXT(""));
		if (ID != FName("2"))
		{
			Mag->SetStaticMesh(AccData->StaticMesh);
		}
		else
		{
			Mag->SetStaticMesh(AccData->StaticMesh2);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// UpdateMuzzle

void AEquipmentsWeapon::UpdateMuzzle(AEquipmentsWeaponAcc* MuzzleAccActor)
{
	AccMuzzleObj= MuzzleAccActor;
	if (AccMuzzleObj == nullptr)
	{
		Muzzle->SetStaticMesh(nullptr);
	}
	else
	{
		FItemWeaponAcc* AccData = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(AccMuzzleObj->ID,TEXT(""));
		Muzzle->SetStaticMesh(AccData->StaticMesh);
	}
}

//////////////////////////////////////////////////////////////////////////
// UpdateSight

void AEquipmentsWeapon::UpdateSight(AEquipmentsWeaponAcc* SightAccActor)
{
	AccSightObj = SightAccActor;
	if (AccSightObj == nullptr)
	{
		Sight->SetStaticMesh(Datas->DefaultSight);
		Sight->K2_AttachToComponent(SkeletalMesh, "Socket_Sight_Default", EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,
							  EAttachmentRule::KeepRelative, true);
	}
	else
	{
		FItemWeaponAcc* AccData = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(AccSightObj->ID,TEXT(""));
		Sight->SetStaticMesh(AccData->StaticMesh);
		Sight->K2_AttachToComponent(SkeletalMesh, AccData->SocketName, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,
							  EAttachmentRule::KeepRelative, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// UpdateForegrip

void AEquipmentsWeapon::UpdateForegrip(AEquipmentsWeaponAcc* ForegripAccActor)
{
	AccForegripObj = ForegripAccActor;
	if (AccForegripObj == nullptr)
	{
		Foregrip->SetStaticMesh(nullptr);
	}
	else
	{
		FItemWeaponAcc* AccData = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(AccForegripObj->ID,TEXT(""));
		Foregrip->SetStaticMesh(AccData->StaticMesh);
	}
}

//////////////////////////////////////////////////////////////////////////
// UpdateFButtstock

void AEquipmentsWeapon::UpdateButtstock(AEquipmentsWeaponAcc* ButtstockAccActor)
{
	AccButtstockObj = ButtstockAccActor;
	if (AccButtstockObj == nullptr)
	{
		Buttstock->SetStaticMesh(Datas->DefaultButtstock);
	}
	else
	{
		FItemWeaponAcc* AccData = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(AccButtstockObj->ID,TEXT(""));
		Buttstock->SetStaticMesh(AccData->StaticMesh);
	}
}

//////////////////////////////////////////////////////////////////////////
// PlayFiringSound

void AEquipmentsWeapon::PlayFiringSound()
{
	if (IsValid(AccMuzzleObj))
	{
		FItemWeaponAcc* AccData = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(AccMuzzleObj->ID,TEXT(""));
		if (AccData->IsSilencer)
		{
			Audio->SetSound(Datas->FireSoundSuppressor);
		}
		else
		{
			Audio->SetSound(Datas->FireSound);
		}
	}
	else
	{
		Audio->SetSound(Datas->FireSound);
	}

	Audio->Play(0.0f);
}

void AEquipmentsWeapon::PlayFireFlash()
{
	if (IsValid(AccMuzzleObj))
	{
		FItemWeaponAcc* AccData = ItemWeaponAccTable->FindRow<FItemWeaponAcc>(AccMuzzleObj->ID,TEXT(""));
		if (!AccData->IsNotFlash)
		{
			FireFlash->SetActive(true, true);
		}
	}
	else
	{
		FireFlash->SetActive(true, true);
	}

	Audio->Play(0.0f);
}
