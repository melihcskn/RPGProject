// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UMainMenu_BaseWidget;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;


public:

	// virtual void DrawHUD() override;

	UMainMenu_BaseWidget* CurrentWidget;

	TArray<UMainMenu_BaseWidget*> WidgetHistory;

	void RemoveWidget();
	
	static FVector2D CrossHairDot;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MenuButtonClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OptionsMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SoundOptionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SoundSliderClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GraphicOptionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OptionSelectionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ContrlOptionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ControlItemClass;
};
