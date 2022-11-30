// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "EnemyBTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPGPROJECT_API UEnemyBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	public:
		UEnemyBTTask_FindRandomLocation(FObjectInitializer const& object_initiliazer);
		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	private:
		UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Search",meta = (AllowPrivateAccess="true"))
			float SearchRadius = 1500.0f;
};
