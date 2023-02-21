// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetBase/WidgetBase.h"
#include "QuestBoardWidget.generated.h"

struct FQuest;
class UTextBlock;
class UVerticalBox;
class UButton;
class UWrapBox;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestBoardWidget : public UWidgetBase
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void OnItemClicked(int32 ItemIndex) override;

	virtual void SetSelectedWidget(UUserWidget* WidgetToSelect) override;

	TWeakObjectPtr<APlayerCharacter> Owner;

	TArray<FQuest> Quests;

	void CreateQuestTabs();

	void SetQuestDetails();

protected:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestExplanationText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestDescriptionText;

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* QuestsIDVerticalBox;

	UPROPERTY(meta=(BindWidget))
	UButton* ExitQuestBoardButton;

	UPROPERTY(meta=(BindWidget))
	UButton* FinishQuestButton;

	UPROPERTY(meta=(BindWidget))
	UButton* AcceptQuestButton;

	UPROPERTY(meta=(BindWidget))
	UWrapBox* QuestItemIconsWrapBox;

	UPROPERTY(meta=(BindWidget))
	UWrapBox* RewardItemIconsWrapBox;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Quest Widget Item")
	TSubclassOf<UUserWidget> QuestBoardWidgetItemClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Quest Widget Item")
	TSubclassOf<UUserWidget> QuestBoardWidgetQuestItemIconClass;

	UFUNCTION()
	void ExitQuestBoard();

	UFUNCTION()
	void AcceptQuest();

	UFUNCTION()
	void FinishQuest();

public:
	
	void SetQuests(TArray<FQuest> QuestsToSet);

	void SetOwner(APlayerCharacter* PlayerCharacterToSet)
	{
		Owner = PlayerCharacterToSet;
		if (Owner.IsValid())
			UE_LOG(LogTemp, Warning, TEXT("Valid"));
	}
};
