// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "MainMenuOptions.generated.h"

class UMainMenu_BaseButton;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenuOptions : public UMainMenu_BaseWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	
public:

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseButton* SoundButton;

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseButton* GraphicsButton;

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseButton* ControlsButton;

	UFUNCTION()
	void OpenSoundOptions();

	UFUNCTION()
	void OpenGraphicsOption();

	UFUNCTION()
	void OpenControlsOption();
	
};
