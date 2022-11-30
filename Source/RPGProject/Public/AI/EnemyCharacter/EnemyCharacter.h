// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "EnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEnemyKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

class AEnemyCharacter_AIController;
class UEnemyWidget;

class AEnemyWeaponProjectile;

UCLASS()
class RPGPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	float WeaponSpreadRatio = 1.0f;

	FAttachmentTransformRules const AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,false);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float const MaxHealth= 100.0f;

	UPROPERTY(BlueprintReadOnly)
	AActor* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	bool EnemySeen;

	UPROPERTY(EditDefaultsOnly,Category="Fire")
	float MaxFireNumber = 30.0f;

	float FireCounter;
	
	float GetCurrentHealth();

	void SetCurrentHealth(float NewHealth);
	
	void Punch();

	void PrepareMeleeAttack();

	void EndMeleeAttack();

	void PlayFire_VFX();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Attack")
	bool IsHoldingWeapon;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Attack")
	bool IsAIAiming;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	UStaticMeshComponent* Weapon;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyKilled OnEnemyDie;

private:

	FName WeaponMuzzleSocketName;

	FVector WeaponMuzzleLocation;

	AEnemyCharacter_AIController* AI_Controller;

	UEnemyWidget* HealthWidget;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Health",meta = (AllowPrivateAccess="true"))
	float CurrentHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Health",meta=(AllowPrivateAccess="true"))
	bool bIsDead=false;
	
	UWidgetComponent* Enemy_Widget;
	
	UAIPerceptionStimuliSourceComponent* Stimulus;

	UPROPERTY(EditDefaultsOnly,Category="Effect",meta=(AllowPrivateAccess="true"))
	UParticleSystem* GunFire;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Animation",meta=(AllowPrivateAccess="true"))
	UAnimMontage* FireMontage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Animation",meta=(AllowPrivateAccess="true"))
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Animation",meta=(AllowPrivateAccess="true"))
	UAnimMontage* PunchAnim;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Animation",meta=(AllowPrivateAccess="true"))
	UAnimMontage* PlayerPunchedAnim;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Collision",meta=(AllowPrivateAccess = "true"))
	USphereComponent* RightFist_BoxComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Collision",meta=(AllowPrivateAccess = "true"))
	USphereComponent* LeftFist_BoxComponent;

	UPROPERTY(EditDefaultsOnly,Category="Projectile")
	TSubclassOf<AEnemyWeaponProjectile> ProjectileClass;
	
	UFUNCTION()
	void OnMeleeAttackHit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
	void Enemy_OnTakeDamage(AActor* DamagedActor,
		float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy,
		AActor* DamageCauser );

	UFUNCTION()
	void OnPlayerSeen(AActor* PlayerActor);
	
	void Setup_Stimulus();

};
