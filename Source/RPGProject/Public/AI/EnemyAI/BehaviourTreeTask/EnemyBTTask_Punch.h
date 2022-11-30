// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyBTTask_Punch.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyBTTask_Punch : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UEnemyBTTask_Punch(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
