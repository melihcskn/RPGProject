// Fill out your copyright notice in the Description page of Project Settings.
#define UP false
#define DOWN true

#include "Widget/WidgetBase/WidgetBase.h"

#include "Player/PlayerCharacter/PlayerHUD.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"

void UWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	VerticalWidgetNavigationStep = 1;
	HorizontalWidgetNavigationStep = 1;
	
	Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->AddWidget(this);
}

void UWidgetBase::SetSelectedWidget(UUserWidget* WidgetToSelect)
{
	if(!SelectedWidget)
	{
		SetSelectedWidget();
	}
	IMenuItemsInterface* TargetMI = Cast<IMenuItemsInterface>(WidgetToSelect);
	IMenuItemsInterface* SelectedMI = Cast<IMenuItemsInterface>(SelectedWidget);
	if(SelectedMI && TargetMI)
	{
		SelectedMI->UnSelect();
		TargetMI->SetSelected();
		SelectedWidget = WidgetToSelect;
	}
}

void UWidgetBase::SetSelectedWidget()
{
	if(!SelectedWidget)
	{
		if(WidgetItems.IsValidIndex(SelectedWidgetItemIndex) && Cast<IMenuItemsInterface>(WidgetItems[SelectedWidgetItemIndex]))
		{
			SelectedWidget = WidgetItems[SelectedWidgetItemIndex];
			Cast<IMenuItemsInterface>(SelectedWidget)->SetSelected();
		}
	}
}

void UWidgetBase::SetFocusOptions()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	SetFocus();
	SetKeyboardFocus();
	SetSelectedWidget();
}

void UWidgetBase::Navigate_UpDown(bool bDirection)
{
	if(bDirection && WidgetItems.IsValidIndex(SelectedWidgetItemIndex + VerticalWidgetNavigationStep))
	{
		SelectedWidgetItemIndex += VerticalWidgetNavigationStep;
		SetSelectedWidget(WidgetItems[SelectedWidgetItemIndex]);
	}
	else if(!bDirection && WidgetItems.IsValidIndex(SelectedWidgetItemIndex - VerticalWidgetNavigationStep))
	{
		SelectedWidgetItemIndex -= VerticalWidgetNavigationStep;
		SetSelectedWidget(WidgetItems[SelectedWidgetItemIndex]);
	}
	else if(!bDirection && WidgetItems.Num()!=0)
	{
		SetSelectedWidget(WidgetItems[0]);
	}
}
