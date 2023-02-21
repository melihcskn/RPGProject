// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Player/Interactable/QuestDataAsset.h"
#include "Player/Interactable/ItemDataAsset.h"

FItem AMyGameModeBase::FindItem(FName ItemID,bool& IsItemFound)
{
	FItem TempItem = FItem();
	IsItemFound=false;
	if(!ItemDatabase) return TempItem;
	for(int i =0;i<ItemDatabase->ItemData.Num();i++)
	{
		if(ItemDatabase->ItemData[i].ItemID == ItemID)
		{
			IsItemFound=true;
			return ItemDatabase->ItemData[i];
		}
	}
	return TempItem;
}

FQuest AMyGameModeBase::FindQuest(FName QuestID, bool& IsQuestFound)
{
	FQuest TempQuest = FQuest();
	IsQuestFound = false;
	if(!QuestDatabase) return TempQuest;
	for(int i = 0; i<QuestDatabase->GetQuest().Num(); i++)
	{
		if(QuestDatabase->GetQuest()[i].QuestID == QuestID)
		{
			IsQuestFound = true;
			return QuestDatabase->GetQuest()[i];
		}
	}
	return TempQuest;
}
