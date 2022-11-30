// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetItemBase/MainMenu_BaseButton.h"

#include "Components/Button.h"

void UMainMenu_BaseButton::NativeConstruct()
{
	Super::NativeConstruct();

	DefaultColour.A = 0.5;
	DefaultColour.R = 0;
	DefaultColour.G = 0.3;
	DefaultColour.B = 0;

	SelectedColour.A = 0.5;
	SelectedColour.R = 0;
	SelectedColour.G = 1;
	SelectedColour.B = 0;
	ItemButton->SetBackgroundColor(DefaultColour);
}

//Set selection parameter for this item
void UMainMenu_BaseButton::SetSelected()
{
	IMenuItemsInterface::SetSelected();

	ItemButton->SetBackgroundColor(SelectedColour);
}

//Set default parameters for this item
void UMainMenu_BaseButton::UnSelect()
{
	IMenuItemsInterface::UnSelect();

	ItemButton->SetBackgroundColor(DefaultColour);
}

void UMainMenu_BaseButton::Apply()
{
	IMenuItemsInterface::Apply();

	ItemButton->OnClicked.Broadcast();
}
