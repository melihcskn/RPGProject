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
	AddWidgetItem(StartGameButton, 0);
	AddWidgetItem(OptionsButton, 1);
	AddWidgetItem(QuitButton, 2);
	
	Super::NativeConstruct();

	bCanBeRemoved = false;
	
	ControlledPawn=PC->GetPawn();
	GI->SetGameState(EGameState::Menu);
	
	StartGameButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));
	OptionsButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));
	QuitButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));

	if(GI->bIsGameStarted)
	{
		Cast<UMainMenu_BaseButton>(WidgetItems[0])->ItemButton->OnClicked.AddDynamic(this,&UMainMenu::UMainMenu::ResumeGame);
		Cast<UMainMenu_BaseButton>(WidgetItems[0])->ItemText->SetText(FText::FromString("Resume Game"));
	}
	else
	{
		Cast<UMainMenu_BaseButton>(WidgetItems[0])->ItemButton->OnClicked.AddDynamic(this,&UMainMenu::UMainMenu::StartGame);
		Cast<UMainMenu_BaseButton>(WidgetItems[0])->ItemText->SetText(FText::FromString("Start Game"));
	}

	Cast<UMainMenu_BaseButton>(WidgetItems[1])->ItemButton->OnClicked.AddDynamic(this,&UMainMenu::UMainMenu::OpenOptions);
	Cast<UMainMenu_BaseButton>(WidgetItems[2])->ItemButton->OnClicked.AddDynamic(this,&UMainMenu::UMainMenu::QuitGame);
	
	Cast<UMainMenu_BaseButton>(WidgetItems[1])->ItemText->SetText(FText::FromString("Options"));
	Cast<UMainMenu_BaseButton>(WidgetItems[2])->ItemText->SetText(FText::FromString("Quit Game"));
	
}

//Set StartGame button
void UMainMenu::StartGame()
{
	GI->bIsGameStarted = true;
	GI->SetGameState(EGameState::Game);
	if(GetPlayerHUD())
	{
		GetPlayerHUD()->RemoveWidget();
		UGameplayStatics::SetGamePaused(this,false);
	}
}

//Set ResumeGame button
void UMainMenu::ResumeGame()
{
	UGameplayStatics::SetGamePaused(this,false);
	GI->SetGameState(EGameState::Game);
	if(GetPlayerHUD())
	{
		GetPlayerHUD()->RemoveWidget();
		UGameplayStatics::SetGamePaused(this,false);
	}
}

//Set OpenOptions button
void UMainMenu::OpenOptions()
{
	if(MyHud->OptionsMenuClass)
	{
		UMainMenu_BaseWidget* OptionsMenu = Cast<UMainMenu_BaseWidget>(CreateWidget(PC,MyHud->OptionsMenuClass));
		MyHud->AddWidget(OptionsMenu);
	}
}

//Set quit game button
void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this,GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,true);
}
