// Fill out your copyright notice in the Description page of Project Settings.
#define UP false
#define DOWN true

#include "Widget/WidgetBase/WidgetBase.h"
#include "WidgetPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"

void UWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;
	bCanBeRemoved = true;

	VerticalWidgetNavigationStep = 1;
	HorizontalWidgetNavigationStep = 1;
}

void UWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
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
		SelectedWidgetItemIndex = TargetMI->GetItemIndex();
	}
}

void UWidgetBase::SetSelectedWidget()
{
	if (!SelectedWidget)
	{
		if (WidgetItems.IsValidIndex(SelectedWidgetItemIndex) && Cast<IMenuItemsInterface>(WidgetItems[SelectedWidgetItemIndex]))
		{
			SelectedWidget = WidgetItems[SelectedWidgetItemIndex];
			Cast<IMenuItemsInterface>(SelectedWidget)->SetSelected();
		}
	}
}


void UWidgetBase::AddWidgetItem(UUserWidget* WidgetItemToAdd,int32 WidgetItemIndex)
{
	WidgetItems.Push(WidgetItemToAdd);
	if(Cast<IMenuItemsInterface>(WidgetItemToAdd))
	{
		Cast<IMenuItemsInterface>(WidgetItemToAdd)->SetItemIndex(WidgetItemIndex);
		Cast<IMenuItemsInterface>(WidgetItemToAdd)->ItemClicked.BindUObject(this, &UWidgetBase::OnItemClicked);
	}
}

FReply UWidgetBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	CheckPressedKeyBehaviour(InKeyEvent.GetKey());

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UWidgetBase::OnItemClicked(int32 ItemIndex)
{
	
}

void UWidgetBase::CheckPressedKeyBehaviour(FKey PressedKey)
{
	AWidgetPlayerController* PC = GetPlayerController();
	APlayerHUD* MyHud = GetPlayerHUD();
	if (!PC || !MyHud)
		return;

	FString PressedKeyName = PressedKey.GetDisplayName().ToString();
	for (int32 i = 0; i < PC->Widget_Up.Num(); i++)
	{
		if (PC->Widget_Up[i].Compare(PressedKeyName) == 0)
		{
			Navigate_UpDown(UP);
		}
	}
	for (int32 i = 0; i < PC->Widget_Down.Num(); i++)
	{
		if (PC->Widget_Down[i].Compare(PressedKeyName) == 0)
		{
			Navigate_UpDown(DOWN);
		}
	}
	for (int32 i = 0; i < PC->Widget_Left.Num(); i++)
	{
		if (PC->Widget_Left[i] == PressedKeyName)
		{
			IMenuSwitchableItemInterface* MIS = Cast<IMenuSwitchableItemInterface>(SelectedWidget);
			if (MIS)
			{
				MIS->SwitchLeft();
			}
		}
	}
	for (int32 i = 0; i < PC->Widget_Right.Num(); i++)
	{
		if (PC->Widget_Right[i] == PressedKeyName)
		{
			IMenuSwitchableItemInterface* MIS = Cast<IMenuSwitchableItemInterface>(SelectedWidget);
			if (MIS)
			{
				MIS->SwitchRight();
			}
		}
	}
	for (int32 i = 0; i < PC->Widget_Apply.Num(); i++)
	{
		if (PC->Widget_Apply[i] == PressedKeyName)
		{
			IMenuItemsInterface* MI = Cast<IMenuItemsInterface>(SelectedWidget);
			if (MI)
			{
				MI->Apply();
			}
		}
	}
	for (int32 i = 0; i < PC->Widget_Back.Num(); i++)
	{
		if (PC->Widget_Back[i] == PressedKeyName && bCanBeRemoved)
		{
			MyHud->RemoveWidget();
		}
	}
}

AWidgetPlayerController* UWidgetBase::GetPlayerController()
{
	return Cast<AWidgetPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

APlayerHUD* UWidgetBase::GetPlayerHUD()
{
	return Cast<APlayerHUD>(GetPlayerController()->GetHUD());
}

void UWidgetBase::SetFocusOptions()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
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
