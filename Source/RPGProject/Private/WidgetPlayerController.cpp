// Fill out your copyright notice in the Description page of Project Settings.
#define UP false
#define DOWN true

#include "WidgetPlayerController.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"
#include "MyGameInstance.h"
#include "Player/PlayerCharacter/PlayerHUD.h"

void AWidgetPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Widget_Up.Push("Up");
	Widget_Down.Push("Down");
	Widget_Apply.Push("Enter");
	Widget_Back.Push("Z");
	Widget_Left.Push("Left");
	Widget_Right.Push("Right");

	GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	MyHud = Cast<APlayerHUD>(MyHUD);
}

void AWidgetPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AWidgetPlayerController::NavigateRight()
{
	IMenuSwitchableItemInterface* MIS = Cast<IMenuSwitchableItemInterface>(MyHud->CurrentWidget->SelectedWidget);
	if(MIS)
	{
		MIS->SwitchRight();
	}
}

void AWidgetPlayerController::NavigateLeft()
{
	IMenuSwitchableItemInterface* MIS = Cast<IMenuSwitchableItemInterface>(MyHud->CurrentWidget->SelectedWidget);
	if(MIS)
	{
		MIS->SwitchLeft();
	}
}

void AWidgetPlayerController::NavigateDown()
{
	MyHud->CurrentWidget->Navigate_UpDown(DOWN);
}

void AWidgetPlayerController::NavigateUp()
{
	MyHud->CurrentWidget->Navigate_UpDown(UP);
}

void AWidgetPlayerController::Apply()
{
	IMenuItemsInterface* MI = Cast<IMenuItemsInterface>(MyHud->CurrentWidget->SelectedWidget);
	if(MI)
	{
		MI->Apply();
	}		
}

void AWidgetPlayerController::Back()
{
	MyHud->RemoveWidget();
}


