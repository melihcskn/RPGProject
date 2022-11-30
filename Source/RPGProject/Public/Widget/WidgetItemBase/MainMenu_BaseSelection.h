// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_BaseSelection.generated.h"

class UButton;
class UTextBlock;
class UBorder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptionChanged);

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenu_BaseSelection : public UUserWidget, public IMenuSwitchableItemInterface, public IMenuItemsInterface
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	void SetButtons();

public:

	uint8 SelectedOption;
	
	TArray<FString> Options;

	UPROPERTY(meta =(BindWidget))
	UButton* SwitchLeftButton;

	UPROPERTY(meta =(BindWidget))
	UButton* SwitchRightButton;

	UPROPERTY(meta =(BindWidget))
	UBorder* SelectionBorder;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SwitchText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemText;

	UPROPERTY()
	FOnOptionChanged OptionChanged;

	virtual void SwitchLeft() override;

	virtual void SwitchRight() override;

	virtual void Apply() override;

	virtual void SetSelected() override;

	virtual void UnSelect() override;

	void SetInitial();
};
