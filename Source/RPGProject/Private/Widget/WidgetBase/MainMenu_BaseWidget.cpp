// Fill out your copyright notice in the Description page of Project Settings.
//When navigating UP means decreasing the number of selected item index, DOWN means increasing the selected item index
#define UP false
#define DOWN true

#include "Widget/WidgetBase/MainMenu_BaseWidget.h"

#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"
#include "MyGameInstance.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "WidgetPlayerController.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu_BaseWidget::NativeConstruct()
{

	Super::NativeConstruct();
	bIsFocusable = true;
	
	SetFocus();
	SelectedItemIndex = 0;
	//Pre casting 
	GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	PC = Cast<AWidgetPlayerController>(UGameplayStatics::GetPlayerController(this,0));
	MyHud = Cast<APlayerHUD>(PC->GetHUD());
	
	if(MyHud)
	{
			MyHud->CurrentWidget = this;
			MyHud->WidgetHistory.Push(this);
	}
	
	Navigate_UpDown(false);
}

FReply UMainMenu_BaseWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	CheckPressedKeyBehaviour(InKeyEvent.GetKey());
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

//Determine pressed key behaviour
void UMainMenu_BaseWidget::CheckPressedKeyBehaviour(FKey PressedKey)
{
	FString PressedKeyName = PressedKey.GetDisplayName().ToString();
	for(int32 i = 0;i<PC->Widget_Up.Num();i++)
	{
		if(PC->Widget_Up[i].Compare(PressedKeyName)==0)
		{
			Navigate_UpDown(UP);
		}
	}
	for(int32 i = 0;i<PC->Widget_Down.Num();i++)
	{
		if(PC->Widget_Down[i].Compare(PressedKeyName)==0)
		{
			Navigate_UpDown(DOWN);
		}
	}
	for(int32 i = 0;i<PC->Widget_Left.Num();i++)
	{
		if(PC->Widget_Left[i]==PressedKeyName)
		{
			IMenuSwitchableItemInterface* MIS = Cast<IMenuSwitchableItemInterface>(SelectedWidget);
			if(MIS)
			{
				MIS->SwitchLeft();
			}
		}
	}
	for(int32 i = 0;i<PC->Widget_Right.Num();i++)
	{
		if(PC->Widget_Right[i]==PressedKeyName)
		{
			IMenuSwitchableItemInterface* MIS = Cast<IMenuSwitchableItemInterface>(SelectedWidget);
			if(MIS)
			{
				MIS->SwitchRight();
			}
		}
	}
	for(int32 i = 0;i<PC->Widget_Apply.Num();i++)
	{
		if(PC->Widget_Apply[i]==PressedKeyName)
		{
			IMenuItemsInterface* MI = Cast<IMenuItemsInterface>(SelectedWidget);
			if(MI)
			{
				MI->Apply();
			}	
		}
	}
	for(int32 i = 0;i<PC->Widget_Back.Num();i++)
	{
		if(PC->Widget_Back[i]==PressedKeyName)
		{
			MyHud->RemoveWidget();
		}
	}
}

//Set focus to selected widget, without any intervention, mouse click etc.
void UMainMenu_BaseWidget::SetFocusOptions()
{
	if(PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		SetKeyboardFocus();
	}
}

//Change selected item
void UMainMenu_BaseWidget::Navigate_UpDown(bool bDirection)
{
	if(bDirection && MenuItems.IsValidIndex(SelectedItemIndex + 1))
	{
		IMenuItemsInterface* MI = Cast<IMenuItemsInterface>(MenuItems[SelectedItemIndex]);
		if(MI)
		{
			MI->UnSelect();
		}
		++SelectedItemIndex;
		MI = Cast<IMenuItemsInterface>(MenuItems[SelectedItemIndex]);
		if(MI)
		{
			MI->SetSelected();
		}
		SelectedWidget = MenuItems[SelectedItemIndex];
	}
	else if (!bDirection && MenuItems.IsValidIndex(SelectedItemIndex-1))
	{
		IMenuItemsInterface* MI = Cast<IMenuItemsInterface>(MenuItems[SelectedItemIndex]);
		if(MI)
		{
			MI->UnSelect();
		}
		--SelectedItemIndex;
		MI = Cast<IMenuItemsInterface>(MenuItems[SelectedItemIndex]);
		if(MI)
		{
			MI->SetSelected();
		}
		SelectedWidget = MenuItems[SelectedItemIndex];
	}
	else if(!bDirection && MenuItems.Num()!=0)
	{
		IMenuItemsInterface* MI = Cast<IMenuItemsInterface>(MenuItems[SelectedItemIndex]);
		if(MI)
		{
			MI->SetSelected();
		}
		SelectedWidget = MenuItems[SelectedItemIndex];
	}

}

