// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Player/Interactable/QuestDataAsset.h"
#include "Components/ActorComponent.h"
#include "PlayerQuests.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnQuestChange);

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UPlayerQuests : public UActorComponent
{
	GENERATED_BODY()

protected:	
	// Sets default values for this component's properties
	UPlayerQuests();

	APlayerCharacter* MyOwner;

	APlayerCharacter* GetQuestOwner();
	
	TArray<FQuest> ActiveQuestList;

	TArray<FQuest> FinishedQuestList;

public:

	FOnQuestChange OnQuestChange;

	TArray<FQuest> GetActiveQuests();

	TArray<FQuest> GetFinishedQuests();

	void AddQuest(FQuest QuestToAdd);

	void RemoveQuest(FName QuestIDToRemove);

	void FinishQuest(FName QuestToFinish);
	
	int32 FindQuestIndex(FName QuestIDToFind, bool& bIsFound);

	bool CheckQuest(FName QuestIDToCheck);
};
