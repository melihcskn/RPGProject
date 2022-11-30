// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/WidgetItemBase/MainMenu_BaseControls.h"

#include "Widget/Widget/MainMenuControlOptions.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "MyGameInstance.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "WidgetPlayerController.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu_BaseControls::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable=true;

	UnSelect();
}

void UMainMenu_BaseControls::Apply()
{
	IMenuItemsInterface::Apply();

	//Without setkeyboardfocus command, NativeOnKeyDown not works for this widget
	SetKeyboardFocus();

	Cast<UMainMenuControlOptions>(MyHud->CurrentWidget)->SetTextVisibility(true);
	
	bIsReadyForKeyInput = true;
}

//Set selection parameter for this item
void UMainMenu_BaseControls::SetSelected()
{
	IMenuItemsInterface::SetSelected();

	SelectionBorder->SetBrushColor(FLinearColor::Red);
}

//Set default parameters for this item
void UMainMenu_BaseControls::UnSelect()
{
	IMenuItemsInterface::UnSelect();

	SelectionBorder->SetBrushColor(FLinearColor::Black);
}

FReply UMainMenu_BaseControls::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	//Call the remap function if user wants to replace any input
	if(bIsReadyForKeyInput)
	{
		ReplaceKey(InKeyEvent.GetKey());
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

//Set index and item type
void UMainMenu_BaseControls::SetItem_Index_Type(uint8 Index, bool Type)
{
	ItemIndex = Index;
	ItemType = Type;
}

//Get item type whether it is action or axis
bool UMainMenu_BaseControls::GetItemType()
{
	return ItemType;
}

//Remap action and axis bindings
void UMainMenu_BaseControls::ReplaceKey(FKey Key)
{
	Cast<UMainMenuControlOptions>(MyHud->CurrentWidget)->SetTextVisibility(false);
	FInputActionKeyMapping TempKey = GI->InputSetting->GetActionMappings()[ItemIndex];
	TempKey.Key = Key;
	GI->InputSetting->RemoveActionMapping(GI->InputSetting->GetActionMappings()[ItemIndex]);
	GI->InputSetting->AddActionMapping(TempKey);
	GI->InputSetting->SaveKeyMappings();
	KeyButtonText->SetText(FText::FromString(Key.ToString()));
	MyHud->CurrentWidget->SetKeyboardFocus();
	MyHud->CurrentWidget->SetFocusOptions();
	bIsReadyForKeyInput=false;
}