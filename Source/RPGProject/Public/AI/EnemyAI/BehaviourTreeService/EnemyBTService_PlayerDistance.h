// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "EnemyBTService_PlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyBTService_PlayerDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta = (AllowPrivateAccess="true"))
	float MeleeAttackDistance = 25.0f;
	
	UEnemyBTService_PlayerDistance();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
