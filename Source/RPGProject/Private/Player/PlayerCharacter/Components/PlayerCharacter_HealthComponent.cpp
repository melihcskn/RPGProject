// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/Components/PlayerCharacter_HealthComponent.h"

//#include <string>

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerCharacter_HealthComponent::UPlayerCharacter_HealthComponent()
{
	DefaultHP = 100.0f;
	CurrentHP = DefaultHP;
	bIsDead = false;
}


// Called when the game starts
void UPlayerCharacter_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if(Owner && !bIsDead)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UPlayerCharacter_HealthComponent::TakeDamage);
	}
}

void UPlayerCharacter_HealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0 || bIsDead)
	{
		return;
	}
	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0.0f, DefaultHP);

	if(CurrentHP <= 0 )
	{
		bIsDead = true;
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if(PC)
		{
			GetOwner()->DisableInput(PC);
		}
		GetOwner()->SetActorEnableCollision(false);
		OnActorKilled.Broadcast(DamagedActor, DamageCauser, InstigatedBy);
	}
	
	OnActorDamaged.Broadcast(Damage);
}

void UPlayerCharacter_HealthComponent::HealActor(float HealAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount,0.0f,100.0f);
}

int32 UPlayerCharacter_HealthComponent::GetCurrentHP()
{
	return CurrentHP;
}

float UPlayerCharacter_HealthComponent::GetDefaultHP()
{
	return DefaultHP;
}
