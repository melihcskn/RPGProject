// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/BehaviourTreeTask/EnemyBTTask_FindRandomLocation.h"

#include "blackboard_keys.h"
#include "AI/EnemyCharacter/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AI/EnemyAI/EnemyCharacter_AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


UEnemyBTTask_FindRandomLocation::UEnemyBTTask_FindRandomLocation(FObjectInitializer const& object_initiliazer)
{
	NodeName = TEXT("Find Random Location");
	
}

EBTNodeResult::Type UEnemyBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* EnemyController = Cast<AEnemyCharacter_AIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyController->GetPawn());

	FVector OriginLocation = Enemy->GetActorLocation();
	FNavLocation RandomLocation;

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if(NavigationSystem->GetRandomPointInNavigableRadius(OriginLocation,SearchRadius,RandomLocation))
	{
		EnemyController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),RandomLocation.Location);
	}

	Enemy->IsAIAiming = false;
	Enemy->EnemySeen = false;
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	
	return EBTNodeResult::Succeeded;
}
