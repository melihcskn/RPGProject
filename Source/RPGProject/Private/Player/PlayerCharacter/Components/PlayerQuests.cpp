// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/Components/PlayerQuests.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"

// Sets default values for this component's properties
UPlayerQuests::UPlayerQuests()
{
	MyOwner = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerQuests::AddQuest(FString QName, int32 ItemQuantity, FString QExplanation, FString QItem, UTexture2D* QItemImage)
{
	PlayerQuestList.Push(FPlayerQuestsStruct(QName,ItemQuantity,QExplanation,QItem,QItemImage));
}

void UPlayerQuests::SetQuest(TArray<FPlayerQuestsStruct> QuestToSet)
{
	PlayerQuestList = QuestToSet;
}

bool UPlayerQuests::IsQuestTaken(FString QuestNameToFind)
{
	if(PlayerQuestList.Num()>0)
	{
		for(int32 i=0;i<PlayerQuestList.Num();i++)
		{
			if(PlayerQuestList[i].QuestName.Compare(QuestNameToFind) == 0)
				return true;
		}
	}
	return false;
}

void UPlayerQuests::RemoveQuest(int32 QuestIndexToRemove)
{
	PlayerQuestList.RemoveAt(QuestIndexToRemove);
}

int32 UPlayerQuests::FindQuest(FString QuestNameToFind)
{
	if(PlayerQuestList.Num()>0)
	{
		for(int32 i=0;i<PlayerQuestList.Num();i++)
		{
			if(PlayerQuestList[i].QuestName.Compare(QuestNameToFind) == 0)
				return i;
		}
	}
	return 999;
}
