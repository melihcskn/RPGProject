// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "PlayerQuests.generated.h"

class APlayerCharacter;

USTRUCT(BlueprintType)
struct FPlayerQuestsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
		FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
		int32 QuestItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
		FString QuestExplanation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
		FString QuestItem;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Struct")
		UTexture2D* QuestItemImage;

	FPlayerQuestsStruct()
	{
		QuestName = "test";
		QuestItemQuantity = 0;
		QuestExplanation = nullptr;
		QuestItem = nullptr;
		QuestItemImage=nullptr;
	}

	FPlayerQuestsStruct(FString QName, int32 ItemQuantity, FString QExplanation, FString QItem, UTexture2D* QItemImage)
	{
		QuestName = QName;
		QuestItemQuantity = ItemQuantity;
		QuestExplanation = QExplanation;
		QuestItem = QItem;
		QuestItemImage = QItemImage;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UPlayerQuests : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerQuests();

protected:

	APlayerCharacter* MyOwner;
	
	UPROPERTY(BlueprintReadOnly)
		TArray<FPlayerQuestsStruct> PlayerQuestList;

	UFUNCTION(BlueprintCallable)
		void AddQuest(FString QName, int32 ItemQuantity, FString QExplanation, FString QItem, UTexture2D* QItemImage);

	UFUNCTION(BlueprintCallable)
		void SetQuest(TArray<FPlayerQuestsStruct> QuestToSet);

	UFUNCTION(BlueprintCallable)
		bool IsQuestTaken(FString QuestNameToFind);

	UFUNCTION(BlueprintCallable)
		void RemoveQuest(int32 QuestIndexToRemove);

	UFUNCTION(BlueprintCallable)
		int32 FindQuest(FString QuestNameToFind);
};
