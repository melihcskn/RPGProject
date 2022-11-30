// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_BaseSlider.generated.h"

class USlider;
class UTextBlock;
class UBorder;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenu_BaseSlider : public UUserWidget, public IMenuItemsInterface, public IMenuSwitchableItemInterface
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void Apply() override;

	virtual void SetSelected() override;

	virtual void UnSelect() override;

	virtual void SwitchLeft() override;

	virtual void SwitchRight() override;

	float SliderValue;

public:

	UPROPERTY(meta = (BindWidget))
	USlider* VolumeSLider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SliderText;

	UPROPERTY(meta =(BindWidget))
	UBorder* SelectionBorder;

	
	
};
