// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/AnimNotify/PlayerCharacter_NotifyState.h"

#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerCharacter_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration,EventReference);
	APlayerCharacter* AnimOwner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if(AnimOwner)
	{
		AnimOwner->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
}

void UPlayerCharacter_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation,EventReference);
	APlayerCharacter* AnimOwner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if(AnimOwner)
	{
		AnimOwner->ReloadMagazine();
	}
}
