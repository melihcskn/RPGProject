// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FQuestItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestItemStruct")
	FName QuestItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestItemStruct")
	int32 QuestItemQuantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestItemStruct")
	UTexture2D* QuestItemIcon;

	FQuestItem()
	{
		QuestItemID = "test";
		QuestItemQuantity = 0;
		QuestItemIcon = nullptr;
	}
	
};

USTRUCT(BlueprintType)
struct FQuest
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestStruct")
	FName QuestID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestStruct")
	FText QuestDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestStruct")
	TArray<FQuestItem> QuestItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestStruct")
	TArray<FQuestItem> Rewards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestStruct")
	int32 GoldReward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestStruct")
	int32 XPReward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestStruct")
	UTexture2D* QuestItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestStruct")
	bool QuestStatus;

	FQuest()
	{
		QuestID = "test";
		QuestDescription = FText::FromName("Quest Item Description");
		QuestItems.Push(FQuestItem());
		Rewards.Push(FQuestItem());
		GoldReward = 0;
		XPReward = 0;
		QuestItemIcon = nullptr;
		QuestStatus=false;
	}
};

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestDataAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item Data")
	TArray<FQuest> QuestData;

public:
	TArray<FQuest> GetQuest();
};
