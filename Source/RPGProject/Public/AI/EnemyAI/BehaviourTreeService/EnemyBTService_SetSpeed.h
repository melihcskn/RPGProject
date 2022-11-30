// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "EnemyBTService_SetSpeed.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyBTService_SetSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:

	UEnemyBTService_SetSpeed();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float WalkSpeed = 200.0f;
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
