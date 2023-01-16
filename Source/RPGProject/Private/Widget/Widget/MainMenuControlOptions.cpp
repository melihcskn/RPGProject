// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Widget/MainMenuControlOptions.h"

#include "Widget/WidgetItemBase/MainMenu_BaseControls.h"
#include "MyGameInstance.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "WidgetPlayerController.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GameFramework/InputSettings.h"

void UMainMenuControlOptions::NativeConstruct()
{
	Super::NativeConstruct();
	
	PrepareVerticalBox();
	
	KeyBindWarningMessage->SetText(FText::FromString("Press any key to bind"));
	KeyBindWarningMessage->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuControlOptions::Navigate_UpDown(bool bDirection)
{
	
	if(WidgetItems.IsValidIndex(SelectedItem))
	{
		SB->ScrollWidgetIntoView(WidgetItems[SelectedItem]);//Set focus of selected option while scrolling up and down
	}
	
	Super::Navigate_UpDown(bDirection);
	
}
/*Setting up vertical box to show both action and axis mapping*/
void UMainMenuControlOptions::PrepareVerticalBox()
{
	if(!GI || !MyHud){return;}
	for(int i = 0 ; i<GI->InputSetting->GetActionMappings().Num();i++)
	{
		UMainMenu_BaseControls* BC = Cast<UMainMenu_BaseControls>(CreateWidget(this,MyHud->ControlItemClass));
		BC->MyHud = MyHud;
		BC->GI = GI;
		BC->ItemText->SetText(FText::FromString(GI->InputSetting->GetActionMappings()[i].ActionName.ToString()));
		BC->KeyButtonText->SetText(FText::FromString(GI->InputSetting->GetActionMappings()[i].Key.ToString()));
		BC->SetItem_Index_Type(i,0);
		UVerticalBoxSlot* VBSlot = VB->AddChildToVerticalBox(BC);
		WidgetItems.Push(Cast<UMainMenu_BaseControls>(VB->GetAllChildren().Last()));
	}
	for(int i =0 ; i<GI->InputSetting->GetAxisMappings().Num();i++)
	{
		UMainMenu_BaseControls* BC = Cast<UMainMenu_BaseControls>(CreateWidget(this,MyHud->ControlItemClass));
		BC->MyHud = MyHud;
		BC->GI = GI;
		BC->ItemText->SetText(FText::FromString(GI->InputSetting->GetAxisMappings()[i].AxisName.ToString()));
		BC->KeyButtonText->SetText(FText::FromString(GI->InputSetting->GetAxisMappings()[i].Key.ToString()));
		BC->SetItem_Index_Type(i,1);
		UVerticalBoxSlot* VBSlot = VB->AddChildToVerticalBox(BC);
		WidgetItems.Push(Cast<UMainMenu_BaseControls>(VB->GetAllChildren().Last()));
	}
	SetFocusOptions();
}
//Pop up a textblock while game expects from player to make a keyboard input
void UMainMenuControlOptions::SetTextVisibility(bool Status)
{
	if(Status)
	{
		KeyBindWarningMessage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		KeyBindWarningMessage->SetVisibility(ESlateVisibility::Hidden);
	}
}
