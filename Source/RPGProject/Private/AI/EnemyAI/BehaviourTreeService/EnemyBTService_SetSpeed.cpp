// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/BehaviourTreeService/EnemyBTService_SetSpeed.h"

#include "AI/EnemyCharacter/EnemyCharacter.h"
#include "AI/EnemyAI/EnemyCharacter_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UEnemyBTService_SetSpeed::UEnemyBTService_SetSpeed()
{
	bNotifyBecomeRelevant=true;
	NodeName=TEXT("Change Speed");
}

void UEnemyBTService_SetSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	AAIController* EnemyController = OwnerComp.GetAIOwner();
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyController->GetPawn());
	if(EnemyCharacter)
	{
		EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}
