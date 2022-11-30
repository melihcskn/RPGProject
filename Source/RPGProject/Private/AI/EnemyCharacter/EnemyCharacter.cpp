// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter/EnemyCharacter.h"

#include "blackboard_keys.h"
#include "DrawDebugHelpers.h"
#include "AI/EnemyCharacter/EnemyWidget.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/EnemyCharacter/Weapon/EnemyWeaponProjectile.h"
#include "AI/EnemyAI/EnemyCharacter_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Setup_Stimulus();
	
	GetCharacterMovement()->MaxWalkSpeed=200;
	GetCharacterMovement()->bUseControllerDesiredRotation=true;
	bUseControllerRotationYaw = false;
	
	OnTakeAnyDamage.AddDynamic(this,&AEnemyCharacter::Enemy_OnTakeDamage);

	RightFist_BoxComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Right Fist Collision"));
	LeftFist_BoxComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Left Fist Collision"));
	Enemy_Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Comp"));
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));

	if(RightFist_BoxComponent)
	{
		RightFist_BoxComponent->SetupAttachment(RootComponent);
		RightFist_BoxComponent->SetWorldScale3D(FVector(0.22f));
	}
	
	if(LeftFist_BoxComponent)
	{
		LeftFist_BoxComponent->SetupAttachment(RootComponent);
		LeftFist_BoxComponent->SetWorldScale3D(FVector(0.22f));
	}
	
	if(Enemy_Widget)
	{
		Enemy_Widget->SetupAttachment(RootComponent);
		Enemy_Widget->SetWidgetSpace(EWidgetSpace::World);
		Enemy_Widget->SetRelativeLocation(FVector(0.0f,0.0f,100.0f));
		Enemy_Widget->SetTwoSided(true);
		Enemy_Widget->SetBlendMode(EWidgetBlendMode::Masked);
		static ConstructorHelpers::FClassFinder<UUserWidget> enemy_widget_class(TEXT("/Game/Blueprint/Enemy/EnemyWidget_BP"));
		if(enemy_widget_class.Succeeded())
		{
			Enemy_Widget->SetWidgetClass(enemy_widget_class.Class);
		}
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AI_Controller = Cast<AEnemyCharacter_AIController>(GetController());
	
	if(RightFist_BoxComponent)
	{
		RightFist_BoxComponent->SetCollisionProfileName("NoCollision");
		RightFist_BoxComponent->SetNotifyRigidBodyCollision(false);
		RightFist_BoxComponent->AttachToComponent(GetMesh(),AttachmentRules,"R_Hand_Fist");
		RightFist_BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AEnemyCharacter::OnMeleeAttackHit);
	}

	if(LeftFist_BoxComponent)
	{
		LeftFist_BoxComponent->SetCollisionProfileName("NoCollision");
		LeftFist_BoxComponent->SetNotifyRigidBodyCollision(false);
		LeftFist_BoxComponent->AttachToComponent(GetMesh(),AttachmentRules,"L_Hand_Fist");
		LeftFist_BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AEnemyCharacter::OnMeleeAttackHit);
	}

	if(Weapon)
	{
		Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"WeaponSocket");
		IsHoldingWeapon = true;
		if(AI_Controller)
		{
			AI_Controller->GetBlackboardComponent()->SetValueAsBool(bb_keys::is_weapon_active,IsHoldingWeapon);
		}
	}
	if(IsValid(AI_Controller))
	{AI_Controller->PlayerCharacterSeen.AddDynamic(this,&AEnemyCharacter::OnPlayerSeen);}
	IsAIAiming=false;
	HealthWidget = Cast<UEnemyWidget>(Enemy_Widget->GetUserWidgetObject());
	if(HealthWidget)
	{
		HealthWidget->Set_Health_Bar(CurrentHealth/MaxHealth);
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::OnMeleeAttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor!=this)
	{
		FDamageEvent DamageEv;
		OtherActor->TakeDamage(1.0f,DamageEv,GetController(),this);
		UE_LOG(LogTemp,Warning,TEXT("Hit"));
		StopAnimMontage(PunchAnim);
		EndMeleeAttack();
	}
}

void AEnemyCharacter::Enemy_OnTakeDamage(AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	
	if(Damage <= 0 || bIsDead)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	HealthWidget->Set_Health_Bar(CurrentHealth/MaxHealth);
	
	if(CurrentHealth <= 0 )
	{
		bIsDead = true;
		if(APlayerController* ThisController = Cast<APlayerController>(GetController()))
		{
			DisableInput(ThisController);
		}
		OnEnemyDie.Broadcast(this,DamageCauser,InstigatedBy);
		if(APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(DamageCauser))
		{
			PlayerChar->DecreaseCoin(-100);
		}
		Enemy_Widget->DestroyComponent();
		PlayAnimMontage(DeathMontage);
		SetLifeSpan(5.0f);
	}
}

void AEnemyCharacter::OnPlayerSeen(AActor* PlayerActor)
{
	PlayerCharacter = PlayerActor;
	EnemySeen = true;
}

void AEnemyCharacter::Setup_Stimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

void AEnemyCharacter::Punch()
{
	if(PunchAnim)
	{
		PlayAnimMontage(PunchAnim,0.8f);
	}
}

void AEnemyCharacter::Fire()
{
	if(!Weapon)return;
	
	if(FireCounter >= MaxFireNumber)
	{
		Weapon->DestroyComponent();
		IsAIAiming=false;
		IsHoldingWeapon = false;
		AI_Controller->GetBlackboardComponent()->SetValueAsBool(bb_keys::is_weapon_active,false);
		return;
	}

	++FireCounter;
	
	FRotator WeaponMuzzleRotation;
	WeaponMuzzleLocation = Weapon->GetSocketLocation("Muzzle");
	WeaponMuzzleRotation = Weapon->GetForwardVector().Rotation();
	FVector ShotDirection = WeaponMuzzleRotation.Vector();
	float HalfRad = FMath::DegreesToRadians(WeaponSpreadRatio);
	ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

		// Spawn the projectile at the muzzle.
	AEnemyWeaponProjectile* Projectile = GetWorld()->SpawnActor<AEnemyWeaponProjectile>(ProjectileClass, WeaponMuzzleLocation, ShotDirection.Rotation(), SpawnParams);
	if (Projectile)
	{
		PlayAnimMontage(FireMontage);
		PlayFire_VFX();
		Projectile->FireInDirection(ShotDirection);
	}
}

void AEnemyCharacter::PrepareMeleeAttack()
{
	RightFist_BoxComponent->SetCollisionProfileName("Weapon");
	RightFist_BoxComponent->SetNotifyRigidBodyCollision(true);

	LeftFist_BoxComponent->SetCollisionProfileName("Weapon");
	LeftFist_BoxComponent->SetNotifyRigidBodyCollision(true);
}

void AEnemyCharacter::EndMeleeAttack()
{
	RightFist_BoxComponent->SetCollisionProfileName("NoCollision");
	RightFist_BoxComponent->SetNotifyRigidBodyCollision(false);

	LeftFist_BoxComponent->SetCollisionProfileName("NoCollision");
	LeftFist_BoxComponent->SetNotifyRigidBodyCollision(false);

}

void AEnemyCharacter::PlayFire_VFX()
{
	if(GunFire)
	{
		UGameplayStatics::SpawnEmitterAttached(GunFire,Weapon,"Muzzle");
	}
}

float AEnemyCharacter::GetCurrentHealth()
{
	return CurrentHealth;
}

void AEnemyCharacter::SetCurrentHealth(float NewHealth)
{
	
	CurrentHealth = FMath::Clamp(NewHealth,0.0f,MaxHealth);
}

