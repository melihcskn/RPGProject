// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "MainMenuGraphicOptions.generated.h"

class UMainMenu_BaseSelection;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenuGraphicOptions : public UMainMenu_BaseWidget
{
	GENERATED_BODY()


protected:

	virtual void NativeConstruct() override;
	
public:

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseSelection* Resolution;

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseSelection* TextureQuality;

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseSelection* AntiAliasingQuality;

	UFUNCTION()
	void SwitchToLeftButton();

	UFUNCTION()
	void SwitchToRightButton();

	UFUNCTION()
	void SetAntiAliasingQuality();

	UFUNCTION()
	void SetTextureQuality();

	UFUNCTION()
	void SetResolution();
	
};
