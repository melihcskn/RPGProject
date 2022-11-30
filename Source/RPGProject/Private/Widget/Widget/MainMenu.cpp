// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/MainMenu.h"

#include "Widget/WidgetItemBase/MainMenu_BaseButton.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "MyGameInstance.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "WidgetPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{

	MenuItems.Push(StartGameButton);
	MenuItems.Push(OptionsButton);
	MenuItems.Push(QuitButton);
	
	Super::NativeConstruct();

	ControlledPawn=PC->GetPawn();
	// PC->UnPossess();
	
	StartGameButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));
	OptionsButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));
	QuitButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));

	if(UGameplayStatics::IsGamePaused(this))
	{
		Cast<UMainMenu_BaseButton>(MenuItems[0])->ItemButton->OnClicked.AddDynamic(this,&UMainMenu::UMainMenu::ResumeGame);
		Cast<UMainMenu_BaseButton>(MenuItems[0])->ItemText->SetText(FText::FromString("Resume Game"));
	}
	else
	{
		Cast<UMainMenu_BaseButton>(MenuItems[0])->ItemButton->OnClicked.AddDynamic(this,&UMainMenu::UMainMenu::StartGame);
		Cast<UMainMenu_BaseButton>(MenuItems[0])->ItemText->SetText(FText::FromString("Start Game"));
	}

	Cast<UMainMenu_BaseButton>(MenuItems[1])->ItemButton->OnClicked.AddDynamic(this,&UMainMenu::UMainMenu::OpenOptions);
	Cast<UMainMenu_BaseButton>(MenuItems[2])->ItemButton->OnClicked.AddDynamic(this,&UMainMenu::UMainMenu::QuitGame);
	
	Cast<UMainMenu_BaseButton>(MenuItems[1])->ItemText->SetText(FText::FromString("Options"));
	Cast<UMainMenu_BaseButton>(MenuItems[2])->ItemText->SetText(FText::FromString("Quit Game"));

	SetFocusOptions();
	
}

//Set StartGame button
void UMainMenu::StartGame()
{
	// if(ControlledPawn)
	// PC->Possess(ControlledPawn);
	UGameplayStatics::OpenLevel(this,"MainMap");
}

//Set ResumeGame button
void UMainMenu::ResumeGame()
{
	// if(ControlledPawn)
	// PC->Possess(ControlledPawn);
	UGameplayStatics::SetGamePaused(this,false);
	RemoveFromParent();
}

//Set OpenOptions button
void UMainMenu::OpenOptions()
{
	if(MyHud->OptionsMenuClass)
	{
		UMainMenu_BaseWidget* BW = Cast<UMainMenu_BaseWidget>(CreateWidget(PC,MyHud->OptionsMenuClass));
		if(BW)
		{
			BW->AddToViewport();
		}
	}
}

//Set quit game button
void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this,GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,true);
}
