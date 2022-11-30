// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyBTTask_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPGPROJECT_API UEnemyBTTask_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UEnemyBTTask_ChasePlayer(FObjectInitializer const& object_intializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
