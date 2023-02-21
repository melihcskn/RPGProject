// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/MainMenuGraphicOptions.h"
#include "Widget/WidgetItemBase/MainMenu_BaseSelection.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuGraphicOptions::NativeConstruct()
{
	AddWidgetItem(Resolution, 0);
	AddWidgetItem(TextureQuality, 1);
	AddWidgetItem(AntiAliasingQuality,2);

	TArray<FIntPoint> Res;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Res);
	for(int32 i =0;i<Res.Num();i++)
	{
		Cast<UMainMenu_BaseSelection>(WidgetItems[0])->Options.Push(FString::FromInt(Res[i].X).Append("*").Append(FString::FromInt(Res[i].Y)));
	}

	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->Options.Push("Low");
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->Options.Push("Medium");
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->Options.Push("High");
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->Options.Push("Very High");
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->Options.Push("Ultra");

	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->Options.Push("Low");
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->Options.Push("Medium");
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->Options.Push("High");
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->Options.Push("Very High");
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->Options.Push("Ultra");
	
	Super::NativeConstruct();

	Resolution = Cast<UMainMenu_BaseSelection>(CreateWidget(this,MyHud->OptionSelectionClass));
	TextureQuality = Cast<UMainMenu_BaseSelection>(CreateWidget(this,MyHud->OptionSelectionClass));
	AntiAliasingQuality = Cast<UMainMenu_BaseSelection>(CreateWidget(this,MyHud->OptionSelectionClass));

	Cast<UMainMenu_BaseSelection>(WidgetItems[0])->ItemText->SetText(FText::FromString("Resolution"));
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->ItemText->SetText(FText::FromString("TextureQuality"));
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->ItemText->SetText(FText::FromString("AntiAliasing Quality"));

	Cast<UMainMenu_BaseSelection>(WidgetItems[0])->SetInitial();
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->SetInitial();
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->SetInitial();

	Cast<UMainMenu_BaseSelection>(WidgetItems[0])->SwitchLeftButton->OnClicked.AddDynamic(this,&UMainMenuGraphicOptions::SwitchToLeftButton);
	Cast<UMainMenu_BaseSelection>(WidgetItems[0])->SwitchRightButton->OnClicked.AddDynamic(this,&UMainMenuGraphicOptions::SwitchToRightButton);
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->SwitchLeftButton->OnClicked.AddDynamic(this,&UMainMenuGraphicOptions::SwitchToLeftButton);
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->SwitchRightButton->OnClicked.AddDynamic(this,&UMainMenuGraphicOptions::SwitchToRightButton);
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->SwitchLeftButton->OnClicked.AddDynamic(this,&UMainMenuGraphicOptions::SwitchToLeftButton);
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->SwitchRightButton->OnClicked.AddDynamic(this,&UMainMenuGraphicOptions::SwitchToRightButton);

	Cast<UMainMenu_BaseSelection>(WidgetItems[0])->OptionChanged.AddDynamic(this,&UMainMenuGraphicOptions::SetResolution);
	Cast<UMainMenu_BaseSelection>(WidgetItems[1])->OptionChanged.AddDynamic(this,&UMainMenuGraphicOptions::SetTextureQuality);
	Cast<UMainMenu_BaseSelection>(WidgetItems[2])->OptionChanged.AddDynamic(this,&UMainMenuGraphicOptions::SetAntiAliasingQuality);
	
	SetFocusOptions();
}

void UMainMenuGraphicOptions::SwitchToLeftButton()
{
	Cast<UMainMenu_BaseSelection>(SelectedWidget)->SwitchLeft();
}

void UMainMenuGraphicOptions::SwitchToRightButton()
{
	Cast<UMainMenu_BaseSelection>(SelectedWidget)->SwitchRight();
}

void UMainMenuGraphicOptions::SetAntiAliasingQuality()
{
	uint8 AAQuality = Cast<UMainMenu_BaseSelection>(WidgetItems[2])->SelectedOption;
	FString AAQualityText = "r.PostProcessAAQuality ";
	AAQualityText.Append(FString::FromInt(AAQuality));
	UKismetSystemLibrary::ExecuteConsoleCommand(this,AAQualityText);
}

void UMainMenuGraphicOptions::SetTextureQuality()
{
	uint8 TQuality = Cast<UMainMenu_BaseSelection>(WidgetItems[1])->SelectedOption;
	FString TextureQualityText = "sg.TextureQuality ";
	TextureQualityText.Append(FString::FromInt(TQuality));
	UKismetSystemLibrary::ExecuteConsoleCommand(this,TextureQualityText);
}

void UMainMenuGraphicOptions::SetResolution()
{
	FString HorizontalResString,VerticalResString;
	Cast<UMainMenu_BaseSelection>(WidgetItems[0])->Options[Cast<UMainMenu_BaseSelection>(WidgetItems[0])->SelectedOption].Split("*",&HorizontalResString,&VerticalResString);
	FString ResolutionText = "r.SetRes ";
	FString ScreenMode;//TODO Add ScreenMode options, Fulscreen,Windowed
	ScreenMode = "f";//f stands for fullscreen
	ResolutionText.Append(HorizontalResString).Append("x").Append(VerticalResString).Append(ScreenMode);
	UKismetSystemLibrary::ExecuteConsoleCommand(this,ResolutionText);
}
