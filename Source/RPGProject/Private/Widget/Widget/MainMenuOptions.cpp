// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/MainMenuOptions.h"
#include "Widget/WidgetItemBase/MainMenu_BaseButton.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "WidgetPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMainMenuOptions::NativeConstruct()
{

	AddWidgetItem(SoundButton, 0);
	AddWidgetItem(GraphicsButton, 1);
	AddWidgetItem(ControlsButton,2);
	
	Super::NativeConstruct();

	SoundButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));
	GraphicsButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));
	ControlsButton = Cast<UMainMenu_BaseButton>(CreateWidget(this,MyHud->MenuButtonClass));

	Cast<UMainMenu_BaseButton>(WidgetItems[0])->ItemText->SetText(FText::FromString("Sound"));
	Cast<UMainMenu_BaseButton>(WidgetItems[1])->ItemText->SetText(FText::FromString("Graphics"));
	Cast<UMainMenu_BaseButton>(WidgetItems[2])->ItemText->SetText(FText::FromString("Controls"));

	Cast<UMainMenu_BaseButton>(WidgetItems[0])->ItemButton->OnClicked.AddDynamic(this,&UMainMenuOptions::OpenSoundOptions);
	Cast<UMainMenu_BaseButton>(WidgetItems[1])->ItemButton->OnClicked.AddDynamic(this,&UMainMenuOptions::OpenGraphicsOption);
	Cast<UMainMenu_BaseButton>(WidgetItems[2])->ItemButton->OnClicked.AddDynamic(this,&UMainMenuOptions::OpenControlsOption);

	SetFocusOptions();
}

void UMainMenuOptions::OpenSoundOptions()
{
	if(MyHud->SoundOptionClass)
	{
		UMainMenu_BaseWidget* BW = Cast<UMainMenu_BaseWidget>(CreateWidget(PC,MyHud->SoundOptionClass));
		MyHud->AddWidget(BW);
	}
}

void UMainMenuOptions::OpenGraphicsOption()
{
	if(MyHud->GraphicOptionClass)
	{
		UMainMenu_BaseWidget* BW = Cast<UMainMenu_BaseWidget>(CreateWidget(PC,MyHud->GraphicOptionClass));
		MyHud->AddWidget(BW);
	}
}

void UMainMenuOptions::OpenControlsOption()
{
	if(MyHud->ContrlOptionClass)
	{
		UUserWidget* BW = Cast<UUserWidget>(CreateWidget(PC,MyHud->ContrlOptionClass));
		MyHud->AddWidget(BW);
	}
}
