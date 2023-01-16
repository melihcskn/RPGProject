// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/MainMenuSoundOptions.h"
#include "Widget/WidgetItemBase/MainMenu_BaseSlider.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacter/PlayerHUD.h"

void UMainMenuSoundOptions::NativeConstruct()
{
	WidgetItems.Push(MasterVolume);
	WidgetItems.Push(EffectVolume);
	WidgetItems.Push(MusicVolume);
	
	Super::NativeConstruct();

	MasterVolume = Cast<UMainMenu_BaseSlider>(CreateWidget(this,MyHud->SoundSliderClass));
	EffectVolume = Cast<UMainMenu_BaseSlider>(CreateWidget(this,MyHud->SoundSliderClass));
	MusicVolume = Cast<UMainMenu_BaseSlider>(CreateWidget(this,MyHud->SoundSliderClass));

	Cast<UMainMenu_BaseSlider>(WidgetItems[0])->SliderText->SetText(FText::FromString("Master Volume"));
	Cast<UMainMenu_BaseSlider>(WidgetItems[1])->SliderText->SetText(FText::FromString("Effect Volume"));
	Cast<UMainMenu_BaseSlider>(WidgetItems[2])->SliderText->SetText(FText::FromString("Music Volume"));

	Cast<UMainMenu_BaseSlider>(WidgetItems[0])->VolumeSLider->OnValueChanged.AddDynamic(this,&UMainMenuSoundOptions::SetMasterSound);
	Cast<UMainMenu_BaseSlider>(WidgetItems[1])->VolumeSLider->OnValueChanged.AddDynamic(this,&UMainMenuSoundOptions::SetEffectSound);
	Cast<UMainMenu_BaseSlider>(WidgetItems[2])->VolumeSLider->OnValueChanged.AddDynamic(this,&UMainMenuSoundOptions::SetMusicSound);

	SetFocusOptions();
}

void UMainMenuSoundOptions::SetMasterSound(float value)
{
	SetSounds(value/100.0f,0);
}

void UMainMenuSoundOptions::SetEffectSound(float value)
{
	SetSounds(value/100.0f,1);
}

void UMainMenuSoundOptions::SetMusicSound(float value)
{
	SetSounds(value/100.0f,2);
}
