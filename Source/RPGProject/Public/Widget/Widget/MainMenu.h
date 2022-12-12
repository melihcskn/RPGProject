// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "MainMenu.generated.h"

class UMainMenu_BaseButton;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenu : public UMainMenu_BaseWidget
{
	GENERATED_BODY()

protected:

	APawn* ControlledPawn;
	
	virtual void NativeConstruct() override;

public:

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseButton* QuitButton;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	UMainMenu_BaseButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UMainMenu_BaseButton* OptionsButton;
	
	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void OpenOptions();

	UFUNCTION()
	void QuitGame();
	
};
