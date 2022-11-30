// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetItemBase/MainMenu_BaseSelection.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMainMenu_BaseSelection::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedOption = 0;

	UnSelect();
}

void UMainMenu_BaseSelection::SetButtons()
{

	if(Options.Num() == 1)
	{
		SwitchLeftButton->SetIsEnabled(false);
		SwitchLeftButton->BackgroundColor.A = 0.4f;
		
		SwitchRightButton->SetIsEnabled(false);
		SwitchRightButton->BackgroundColor.A = 0.4f;
	}
	else if(SelectedOption == 0)
	{
		SwitchLeftButton->SetIsEnabled(false);
		SwitchLeftButton->BackgroundColor.A = 0.4f;
	}
	else if(SelectedOption == Options.Num()-1)
	{
		SwitchRightButton->SetIsEnabled(false);
		SwitchRightButton->BackgroundColor.A = 0.4f;
	}
	else
	{
		SwitchLeftButton->SetIsEnabled(true);
		SwitchLeftButton->BackgroundColor.A = 1;
		SwitchRightButton->SetIsEnabled(true);
		SwitchRightButton->BackgroundColor.A = 1;
	}
}

void UMainMenu_BaseSelection::SwitchLeft()
{
	IMenuSwitchableItemInterface::SwitchLeft();

	if(Options.IsValidIndex(SelectedOption-1))
	{
		--SelectedOption;
		SwitchText->SetText(FText::FromString(Options[SelectedOption]));
		SetButtons();
	}
}

void UMainMenu_BaseSelection::SwitchRight()
{
	IMenuSwitchableItemInterface::SwitchRight();

	if(Options.IsValidIndex(SelectedOption+1))
	{
		++SelectedOption;
		SwitchText->SetText(FText::FromString(Options[SelectedOption]));
		SetButtons();
	}
}

void UMainMenu_BaseSelection::Apply()
{
	IMenuItemsInterface::Apply();

	OptionChanged.Broadcast();
}

void UMainMenu_BaseSelection::SetSelected()
{
	IMenuItemsInterface::SetSelected();

	SelectionBorder->SetBrushColor(FLinearColor::Red);
}

void UMainMenu_BaseSelection::UnSelect()
{
	IMenuItemsInterface::UnSelect();

	SelectionBorder->SetBrushColor(FLinearColor::Black);
}

void UMainMenu_BaseSelection::SetInitial()
{
	if(Options.IsValidIndex(SelectedOption))
		SwitchText->SetText(FText::FromString(Options[SelectedOption]));
	
	SetButtons();
}
