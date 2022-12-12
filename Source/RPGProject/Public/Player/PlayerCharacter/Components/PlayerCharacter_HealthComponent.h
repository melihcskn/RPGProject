// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacter_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDamaged, float , NewHP);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UPlayerCharacter_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerCharacter_HealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		bool bIsDead;
	
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float DefaultHP;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
		float CurrentHP;

public:

	UPROPERTY(BlueprintAssignable)
		FOnActorKilled OnActorKilled;

	UPROPERTY()
		FOnActorDamaged OnActorDamaged;
	
	UFUNCTION(BlueprintCallable)
		void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void HealActor(float HealAmount);

	int32 GetCurrentHP();

	float GetDefaultHP();
	
};
