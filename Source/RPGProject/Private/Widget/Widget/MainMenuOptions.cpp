// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/MainMenuOptions.h"
#include "Widget/WidgetItemBase/MainMenu_BaseButton.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "WidgetPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMainMenuOptions::NativeConstruct()
{
	WidgetItems.Push(SoundButton);
	WidgetItems.Push(GraphicsButton);
	WidgetItems.Push(ControlsButton);
	
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
		if(BW)
		{
			BW->AddToViewport();
		}
	}
}

void UMainMenuOptions::OpenGraphicsOption()
{
	if(MyHud->GraphicOptionClass)
	{
		UMainMenu_BaseWidget* BW = Cast<UMainMenu_BaseWidget>(CreateWidget(PC,MyHud->GraphicOptionClass));
		if(BW)
		{
			BW->AddToViewport();
		}
	}
}

void UMainMenuOptions::OpenControlsOption()
{
	if(MyHud->ContrlOptionClass)
	{
		UMainMenu_BaseWidget* BW = Cast<UMainMenu_BaseWidget>(CreateWidget(PC,MyHud->ContrlOptionClass));
		if(BW)
		{
			BW->AddToViewport();
		}
	}
}
