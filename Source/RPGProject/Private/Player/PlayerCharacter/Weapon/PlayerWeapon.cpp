// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/Weapon/PlayerWeapon.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


// Sets default values
APlayerWeapon::APlayerWeapon()
{
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(SkeletalMeshComp);

	SightComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sight"));
	SightComponent->SetupAttachment(SkeletalMeshComp, "Sight");
	SightComponent->SetSimulatePhysics(false);

	MagazineComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));

	WeaponADSSocketName = "ADSSocket";
	MuzzleSocketName = "b_gun_muzzleflash";
	LineTraceDistance = 5000;
	WeaponFireRate = 600;
	bIsAutomaticModeOn = false;
	WeaponDamage = 20.0f;
	PitchRecoil = 0.0f;
	YawRecoil = 0.0f;
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	RemainBulletInMag=GetMaximumMagazine();
}

float APlayerWeapon::GetLineTraceDistance()
{
	return LineTraceDistance;
}

float APlayerWeapon::GetBulletInSec()
{
	return 60/WeaponFireRate;
}

bool APlayerWeapon::GetWeaponFireModeInfo()
{
	return bCanBeAutomatic;
}

void APlayerWeapon::SetWeaponFireMode(bool NewFireMode)
{
	bIsAutomaticModeOn = NewFireMode;
}

uint8 APlayerWeapon::GetRemainBulletInMag()
{
	return RemainBulletInMag;
}

void APlayerWeapon::SetRemainingBulletInMag(uint8 NewRemainingBulletInMag)
{
	RemainBulletInMag = NewRemainingBulletInMag;
}

void APlayerWeapon::Fire_VFX()
{
	if(FireFlash)
	{
		MuzzleLocation = SkeletalMeshComp->GetSocketLocation(MuzzleSocketName);
		UGameplayStatics::SpawnEmitterAttached(FireFlash, SkeletalMeshComp,MuzzleSocketName);
	}
}

void APlayerWeapon::Impact_VFX(FVector ImpactPoint)
{
	if(APlayerWeapon::DefaultFireParticle)
	{
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(this, DefaultFireParticle, ImpactPoint, ShotDirection.Rotation());
	}
}

void APlayerWeapon::Impact_VFX(FHitResult PlayerCharacterHitResult)
{
	FVector ShotDirection = PlayerCharacterHitResult.ImpactPoint - MuzzleLocation;
	ShotDirection.Normalize();
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PlayerCharacterHitResult.PhysMaterial.Get());
	UE_LOG(LogTemp,Warning,TEXT("%s"),*PlayerCharacterHitResult.PhysMaterial.Get()->GetName());
	UParticleSystem* DefaultHitParticle = nullptr;
	switch (SurfaceType)
	{
		case SurfaceType1:
			DefaultHitParticle = MeatHitParticle;
			break;
		default:
			DefaultHitParticle = nullptr;
			break;
	}
	UGameplayStatics::SpawnEmitterAtLocation(this, DefaultHitParticle, PlayerCharacterHitResult.ImpactPoint, ShotDirection.Rotation());
}

void APlayerWeapon::PlayFireSound()
{
	if(WeaponFireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponFireSound, GetActorLocation(),0.3);
	}
}

bool APlayerWeapon::GetWeaponFireMode()
{
	return bIsAutomaticModeOn;
}

uint8 APlayerWeapon::GetMaximumMagazine()
{
	return MagSize;
}

FVector APlayerWeapon::GetMuzzleSocketLocation()
{
	return SkeletalMeshComp->GetSocketLocation(MuzzleSocketName);
}

float APlayerWeapon::GetWeaponDamage()
{
	return WeaponDamage;
}

USkeletalMeshComponent* APlayerWeapon::GetWeaponMesh()
{
	if(SkeletalMeshComp)
	{return SkeletalMeshComp;}
	return nullptr;
}

FName APlayerWeapon::GetADSSocketName()
{
	return WeaponADSSocketName;
}

FRotator APlayerWeapon::GetADSRotation()
{
	return SkeletalMeshComp->GetForwardVector().Rotation();
}

float APlayerWeapon::GetWeaponYawRecoil()
{
	return YawRecoil;
}

float APlayerWeapon::GetWeaponPitchRecoil()
{
	return PitchRecoil;
}

float APlayerWeapon::GetWeaponSpreadRatio()
{
	return WeaponSpreadRatio;
}

