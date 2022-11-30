// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/BehaviourTreeTask/EnemyBTTask_Punch.h"

#include "AI/EnemyCharacter/EnemyCharacter.h"
#include "AI/EnemyAI/EnemyCharacter_AIController.h"

UEnemyBTTask_Punch::UEnemyBTTask_Punch(FObjectInitializer const& object_initializer)
{
	
}

EBTNodeResult::Type UEnemyBTTask_Punch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter_AIController* EnemyController = Cast<AEnemyCharacter_AIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyController->GetPawn());
	EnemyCharacter-> Punch();

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
