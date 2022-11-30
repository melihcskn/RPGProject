// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_BaseWidget.generated.h"

class IMenuItemsInterface;
class UVerticalBox;
class UMyGameInstance;
class APlayerHUD;
class AWidgetPlayerController;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenu_BaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void CheckPressedKeyBehaviour(FKey PressedKey);

	UMyGameInstance* GI;

	uint8 SelectedItemIndex;

	APlayerHUD* MyHud;

	AWidgetPlayerController* PC;

public:

	void SetFocusOptions();
	
	TArray<UUserWidget*> MenuItems;

	UUserWidget* SelectedWidget;
	
	virtual void Navigate_UpDown(bool bDirection);
	
};
