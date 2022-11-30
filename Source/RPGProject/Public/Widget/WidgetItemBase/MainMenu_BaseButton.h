// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_BaseButton.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenu_BaseButton : public UUserWidget, public IMenuItemsInterface
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	FLinearColor DefaultColour, SelectedColour;

public:

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemText;

	virtual void SetSelected() override;

	virtual void UnSelect() override;

	virtual void Apply() override;
};
