// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyBTTask_Fire.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyBTTask_Fire : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UEnemyBTTask_Fire();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
