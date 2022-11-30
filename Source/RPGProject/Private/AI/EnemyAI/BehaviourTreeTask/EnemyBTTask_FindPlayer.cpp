// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/BehaviourTreeTask/EnemyBTTask_FindPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AI/EnemyAI/EnemyCharacter_AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UEnemyBTTask_FindPlayer::UEnemyBTTask_FindPlayer(FObjectInitializer const& object_initializer)
{
	NodeName= TEXT("Find Player");
}

EBTNodeResult::Type UEnemyBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	AAIController* EnemyController = Cast<AEnemyCharacter_AIController>(OwnerComp.GetAIOwner());

	FVector PlayerLocation = Player->GetActorLocation();
	if(SearchRandom)
	{
		FNavLocation loc;

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if(NavSystem->GetRandomPointInNavigableRadius(PlayerLocation,SearchRadius,loc))
		{
			EnemyController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),loc.Location);
		}
	}
	else
	{
		EnemyController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),PlayerLocation);
	}

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
