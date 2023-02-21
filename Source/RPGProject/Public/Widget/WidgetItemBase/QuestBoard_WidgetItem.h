// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "QuestBoard_WidgetItem.generated.h"

class UBorder;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestBoard_WidgetItem : public UUserWidget, public IMenuItemsInterface
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void Apply() override;

	virtual void SetSelected() override;

	virtual void UnSelect() override;

	void SetButtonStyles();

protected:

	UFUNCTION(BlueprintCallable)
	void ItemButtonClicked();

	UPROPERTY(meta=(BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemText;

	UPROPERTY(meta=(BindWidget))
	UButton* ItemButton;

public:

	void SetItemText(FName ItemNameToSet);
};
