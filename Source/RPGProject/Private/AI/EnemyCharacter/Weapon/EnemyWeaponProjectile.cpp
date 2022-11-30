// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter/Weapon/EnemyWeaponProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AEnemyWeaponProjectile::AEnemyWeaponProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	}

	if(!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Comp"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		RootComponent = CollisionComponent;
	}

	if(!ProjectileMovementComp)
	{
		ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
		ProjectileMovementComp->SetUpdatedComponent(RootComponent);
		ProjectileMovementComp->InitialSpeed = 3000.0f;
		ProjectileMovementComp->MaxSpeed = 3000.0f;
		ProjectileMovementComp->bRotationFollowsVelocity = true;
		ProjectileMovementComp->bShouldBounce = false;
		ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	}

	/*ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	ProjectileMeshComp->SetSimulatePhysics(false);*/
	CollisionComponent->OnComponentHit.AddDynamic(this,&AEnemyWeaponProjectile::OnHit);
}

// Called when the game starts or when spawned
void AEnemyWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyWeaponProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComp->Velocity = ShootDirection*ProjectileMovementComp->InitialSpeed;
}

void AEnemyWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if(OtherActor != GetOwner())
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(	Hit.PhysMaterial.Get());
		UParticleSystem* DefaultImpactParticle = nullptr;
		UE_LOG(LogTemp,Warning,TEXT("%s"),*Hit.BoneName.ToString());
		switch (SurfaceType)
		{
			case SurfaceType1:
				DefaultImpactParticle = MeatHitParticle;
				break;
			default:
				DefaultImpactParticle = nullptr;
				break;
		}
		UGameplayStatics::SpawnEmitterAtLocation(this,DefaultImpactParticle,Hit.ImpactPoint,Hit.ImpactNormal.Rotation());
		FDamageEvent DamageEv;
		OtherActor->TakeDamage(ProjectileDamage,DamageEv,GetOwner()->GetInstigatorController(),GetOwner());
		Destroy();
	}
}

