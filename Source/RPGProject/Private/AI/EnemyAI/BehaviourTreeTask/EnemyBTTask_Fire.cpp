// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/BehaviourTreeTask/EnemyBTTask_Fire.h"

#include "AI/EnemyAI/EnemyCharacter_AIController.h"
#include "AI/EnemyCharacter/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyBTTask_Fire::UEnemyBTTask_Fire()
{
	NodeName="Fire";
}

EBTNodeResult::Type UEnemyBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter_AIController* Enemy_Cont = Cast<AEnemyCharacter_AIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* Enemy_Char = Cast<AEnemyCharacter>(Enemy_Cont->GetPawn());

	if(Enemy_Char)
	{
			Enemy_Char->IsAIAiming=true;
			Enemy_Char->Fire();
	}

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
