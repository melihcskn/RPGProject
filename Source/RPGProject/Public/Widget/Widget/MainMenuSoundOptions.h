// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "MainMenuSoundOptions.generated.h"

class UMainMenu_BaseSlider;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenuSoundOptions : public UMainMenu_BaseWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

public:

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseSlider* MasterVolume;

	UPROPERTY(meta =(BindWidget))
	UMainMenu_BaseSlider* EffectVolume;

	UPROPERTY(meta =(BindWidget))
	UMainMenu_BaseSlider* MusicVolume;

	UFUNCTION()
	void SetMasterSound(float value);

	UFUNCTION()
	void SetEffectSound(float value);

	UFUNCTION()
	void SetMusicSound(float value);

	UFUNCTION(BlueprintImplementableEvent)
	void SetSounds(float value, int32 selection);
};
