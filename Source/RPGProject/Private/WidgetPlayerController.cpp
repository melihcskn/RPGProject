// Fill out your copyright notice in the Description page of Project Settings.
#define UP false
#define DOWN true

#include "WidgetPlayerController.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Interactable/QuestDataAsset.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "Widget/Widget/QuestBoardWidget.h"

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
	InputComponent->BindAction("OpenMainMenu",IE_Pressed,this,&AWidgetPlayerController::OpenMainMenu);
	InputComponent->BindAction("OpenQuests",IE_Pressed,this,&AWidgetPlayerController::OpenQuests);
}

void AWidgetPlayerController::OpenMainMenu()
{
	if(GetPawn())
	{
		PossessedPawn = Cast<APlayerCharacter>(GetPawn());
	}
	else if(!PossessedPawn)
	{
		return;
	}
	if(MyHud->MainMenuClass)
	{
		UUserWidget* MainMenuWidget = Cast<UUserWidget>(CreateWidget(this,MyHud->MainMenuClass));
		if(MainMenuWidget)
		{
			UGameplayStatics::SetGamePaused(this,true);
			PossessedPawn->AddWidget(MainMenuWidget);
		}
	}
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
	if(MyHud->PlayerInventoryWidgetClass)
	{
		UUserWidget* PlayerInventoryWidget = Cast<UUserWidget>(CreateWidget(this,MyHud->PlayerInventoryWidgetClass));
		PossessedPawn->AddWidget(PlayerInventoryWidget);
	}
}

void AWidgetPlayerController::OpenQuests()
{
	if(GetPawn())
	{
		PossessedPawn = Cast<APlayerCharacter>(GetPawn());
	}
	else if(!PossessedPawn)
	{
		return;
	}
	if(MyHud->PlayerQuestsWidgetClass)
	{
		UUserWidget* PlayerQuestsWidget = Cast<UUserWidget>(CreateWidget(this,MyHud->PlayerQuestsWidgetClass));
		if(UQuestBoardWidget* CastWidget = Cast<UQuestBoardWidget>(PlayerQuestsWidget))
		{
			CastWidget->SetQuests(PossessedPawn->GetActiveQuests());
			CastWidget->SetOwner(PossessedPawn);
			PossessedPawn->AddWidget(PlayerQuestsWidget);
		}
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

void AWidgetPlayerController::Apply()
{
	IMenuItemsInterface* MI = Cast<IMenuItemsInterface>(MyHud->CurrentWidget->SelectedWidget);
	if(MI)
	{
		MI->Apply();
	}		
}


