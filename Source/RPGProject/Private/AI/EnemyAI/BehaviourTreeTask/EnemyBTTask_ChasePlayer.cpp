// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/BehaviourTreeTask/EnemyBTTask_ChasePlayer.h"

#include "blackboard_keys.h"
#include "AI/EnemyCharacter/EnemyCharacter.h"
#include "AI/EnemyAI/EnemyCharacter_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

UEnemyBTTask_ChasePlayer::UEnemyBTTask_ChasePlayer(FObjectInitializer const& object_intializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UEnemyBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AEnemyCharacter_AIController* EnemyController = Cast<AEnemyCharacter_AIController>(OwnerComp.GetAIOwner());
	FVector PlayerLocation = EnemyController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(EnemyController,PlayerLocation);

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	
	return EBTNodeResult::Succeeded;
}
