// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class UItemDataAsset;
class UQuestDataAsset;
struct FQuest;
struct FItem;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="ItemDatabase")
	UItemDataAsset* ItemDatabase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="QuestDatabase")
	UQuestDataAsset* QuestDatabase;

public:

	FItem FindItem(FName ItemID,bool& IsItemFound);

	FQuest FindQuest(FName QuestID, bool& IsQuestFound);
};
