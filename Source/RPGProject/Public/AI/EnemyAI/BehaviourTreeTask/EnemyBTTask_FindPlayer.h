// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EnemyBTTask_FindPlayer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPGPROJECT_API UEnemyBTTask_FindPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	public:
		UEnemyBTTask_FindPlayer(FObjectInitializer const& object_initializer);

		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	protected:

		UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Search",meta=(AllowProtectedAccess="true"))
		bool SearchRandom = false;

		UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Search",meta=(AllowProtectedAccess="true"))
		float SearchRadius = 150.0f;
		
};
