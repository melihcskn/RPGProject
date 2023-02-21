// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_BaseWidget.generated.h"

class IMenuItemsInterface;
class UVerticalBox;
class UMyGameInstance;
class APlayerHUD;
class AWidgetPlayerController;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenu_BaseWidget : public UWidgetBase
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
	UMyGameInstance* GI;

	APlayerHUD* MyHud;

	AWidgetPlayerController* PC;
	
};
