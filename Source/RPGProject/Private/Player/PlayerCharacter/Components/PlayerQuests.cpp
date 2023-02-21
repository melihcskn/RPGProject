// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/Components/PlayerQuests.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Player/PlayerCharacter/Components/PlayerInventory.h"

// Sets default values for this component's properties
UPlayerQuests::UPlayerQuests()
{
}

APlayerCharacter* UPlayerQuests::GetQuestOwner()
{
	return Cast<APlayerCharacter>(GetOwner());
}

TArray<FQuest> UPlayerQuests::GetActiveQuests()
{
	return ActiveQuestList;
}

TArray<FQuest> UPlayerQuests::GetFinishedQuests()
{
	return FinishedQuestList;
}

void UPlayerQuests::AddQuest(FQuest QuestToAdd)
{
	bool bIsFound;
	int32 QuestIndex = FindQuestIndex(QuestToAdd.QuestID,bIsFound);
	if(!bIsFound)
	{
		ActiveQuestList.Push(QuestToAdd);
		OnQuestChange.Broadcast();
	}
}

void UPlayerQuests::RemoveQuest(FName QuestIDToRemove)
{
	bool bIsFound;
	int32 QuestIndex = FindQuestIndex(QuestIDToRemove, bIsFound);
	if(bIsFound)
	{
		ActiveQuestList.RemoveAt(QuestIndex);
	}
}

void UPlayerQuests::FinishQuest(FName QuestToFinish)
{
	if(CheckQuest(QuestToFinish) && GetQuestOwner())
	{
		bool bIsFound;
		const int32 QuestIndex = FindQuestIndex(QuestToFinish,bIsFound);
		if(bIsFound)
		{
			UE_LOG(LogTemp,Warning,TEXT("1"));
			for(int i =0;i<ActiveQuestList[QuestIndex].QuestItems.Num();i++)
			{
				const FName QuestItemID = ActiveQuestList[QuestIndex].QuestItems[i].QuestItemID;
				const int32 QuestItemQuantity = ActiveQuestList[QuestIndex].QuestItems[i].QuestItemQuantity;
				GetQuestOwner()->GetInventoryComponent()->RemoveItem(QuestItemID,QuestItemQuantity);
			}
			
			for(int i = 0; i<ActiveQuestList[QuestIndex].Rewards.Num();i++)
			{
				const FName RewardItemID = ActiveQuestList[QuestIndex].Rewards[i].QuestItemID;
				const int32 RewardItemQuantity = ActiveQuestList[QuestIndex].Rewards[i].QuestItemQuantity;
				GetQuestOwner()->AddItemToPlayerInventory(RewardItemID,RewardItemQuantity);
			}
			
			bool bIsFinishedBefore;
			FindQuestIndex(QuestToFinish,bIsFinishedBefore);
			
			if(!bIsFinishedBefore)
				FinishedQuestList.Add(ActiveQuestList[QuestIndex]);
			
			ActiveQuestList.RemoveAt(QuestIndex);
			UE_LOG(LogTemp,Warning,TEXT("Quest Finished"));
		}
	}
}

int32 UPlayerQuests::FindQuestIndex(FName QuestIDToFind, bool& bIsFound)
{
	bIsFound=false;
	for(int i =0;i<ActiveQuestList.Num();i++)
	{
		if(ActiveQuestList[i].QuestID == QuestIDToFind)
		{
			bIsFound=true;
			return i;
		}
	}
	return bIsFound;
}

bool UPlayerQuests::CheckQuest(FName QuestIDToCheck)
{
	if(!GetQuestOwner() && !QuestIDToCheck.IsValid())
		return false;
	
	bool bIsFound;
	const int32 QuestIndex = FindQuestIndex(QuestIDToCheck,bIsFound);
	if(bIsFound && !ActiveQuestList[QuestIndex].QuestStatus)
	{
		if(UPlayerInventory* Inventory = GetQuestOwner()->GetInventoryComponent())
		{
			int32 FinishedItems=0;
			for(int i =0;i<ActiveQuestList[QuestIndex].QuestItems.Num();i++)
			{
				bool bIsItemFound;
				const int32 ItemIndex = Inventory->ItemIndexFinder(ActiveQuestList[i].QuestItems[i].QuestItemID,bIsItemFound);
				if(bIsItemFound)
				{
					if(Inventory->GetInventoryItems()[ItemIndex].ItemQuantity >= ActiveQuestList[i].QuestItems[i].QuestItemQuantity)
						FinishedItems++;
				}
			}
			if(FinishedItems == ActiveQuestList[QuestIndex].QuestItems.Num())
				return true;
		}
	}
	return false;
}

