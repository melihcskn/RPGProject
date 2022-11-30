// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

class USpringArmComponent;
class UCameraComponent;
UCLASS(Blueprintable)
class RPGPROJECT_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

protected:

	virtual void BeginPlay() override;

	FName MuzzleSocketName;

	FVector MuzzleLocation;

	FName WeaponADSSocketName;

	FCollisionQueryParams QueryParams;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float PitchRecoil;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float YawRecoil;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float WeaponDamage;
	
	UPROPERTY(EditDefaultsOnly,Category="Weapon")
		float LineTraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool bCanBeAutomatic;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
		bool bIsAutomaticModeOn;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
		float WeaponFireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		uint8 MagSize;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float WeaponSpreadRatio;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
		FString SetWeaponName="Gun";

	uint8 RemainBulletInMag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SightComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MagazineComponent;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
		UParticleSystem* FireFlash;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		UParticleSystem* DefaultFireParticle;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
		UParticleSystem* MeatHitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		USoundBase* WeaponFireSound;

public:
	
	void Fire_VFX();

	void Impact_VFX(FVector ImpactPoint);
	
	void Impact_VFX(FHitResult PlayerCharacterHitResult);

	void PlayFireSound();

	bool GetWeaponFireMode();
	
	float GetLineTraceDistance();

	float GetBulletInSec();

	bool GetWeaponFireModeInfo();

	void SetWeaponFireMode(bool NewFireMode);

	uint8 GetRemainBulletInMag();

	uint8 GetMaximumMagazine();

	void SetRemainingBulletInMag(uint8 NewRemainingBulletInMag);

	FVector GetMuzzleSocketLocation();

	float GetWeaponDamage();

	USkeletalMeshComponent* GetWeaponMesh();

	FName GetADSSocketName();
	
    FRotator GetADSRotation();

	float GetWeaponYawRecoil();

	float GetWeaponPitchRecoil();

	float GetWeaponSpreadRatio();
};
