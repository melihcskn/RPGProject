// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter/AnimNotify/EnemyMeleeAttackAnimNotifyState.h"

#include "AI/EnemyCharacter/EnemyCharacter.h"

void UEnemyMeleeAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(MeshComp->GetOwner());
	if(EnemyChar)
	{
		EnemyChar->PrepareMeleeAttack();
	}
}

void UEnemyMeleeAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(MeshComp->GetOwner());
	if(EnemyChar)
	{
		EnemyChar->EndMeleeAttack();
	}
}
