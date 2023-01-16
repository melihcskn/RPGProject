// Fill out your copyright notice in the Description page of Project Settings.
#define UP false
#define DOWN true

#include "WidgetPlayerController.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"
#include "MyGameInstance.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
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

	InputComponent->BindAction("Inventory",IE_Pressed,this,&AWidgetPlayerController::OpenInventory);
}

void AWidgetPlayerController::OpenInventory()
{
	if(GetPawn())
	{
		PossessedPawn = Cast<APlayerCharacter>(GetPawn());
	}
	else if(!PossessedPawn)
	{
		return;
	}
	if(MyHud->PlayerInventoryWidgetClass && !CurrentWidget )
	{
		UUserWidget* PlayerInventoryWidget = Cast<UUserWidget>(CreateWidget(this,MyHud->PlayerInventoryWidgetClass));
		if(PlayerInventoryWidget)
		{
			PlayerInventoryWidget->AddToViewport();
			CurrentWidget = PlayerInventoryWidget;
			SetShowMouseCursor(true);
			SetInputMode(FInputModeGameAndUI());
		}
		// UnPossess();
	}
	else if(CurrentWidget->GetClass() == MyHud->PlayerInventoryWidgetClass)
	{
		// Possess(PossessedPawn);
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
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


