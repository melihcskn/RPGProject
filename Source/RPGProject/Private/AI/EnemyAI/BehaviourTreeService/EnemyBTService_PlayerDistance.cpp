// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/BehaviourTreeService/EnemyBTService_PlayerDistance.h"

#include "blackboard_keys.h"
#include "AI/EnemyCharacter/EnemyCharacter.h"
#include "AI/EnemyAI/EnemyCharacter_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"

UEnemyBTService_PlayerDistance::UEnemyBTService_PlayerDistance()
{
	bNotifyBecomeRelevant=true;

	NodeName = "Find Player Distance";
}

void UEnemyBTService_PlayerDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if(AEnemyCharacter_AIController* EnemyCont = Cast<AEnemyCharacter_AIController>(OwnerComp.GetAIOwner()))
	{
		if(AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(EnemyCont->GetPawn()))
		{
			if(APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
			{
				bool is_in_range = false;
				if(MeleeAttackDistance >= EnemyChar->GetDistanceTo(PlayerChar))
					is_in_range = true;
				else
				{
					is_in_range = false;
				}
				EnemyCont->GetBlackboard()->SetValueAsBool(GetSelectedBlackboardKey(),is_in_range);
			}
		}
	}
}
