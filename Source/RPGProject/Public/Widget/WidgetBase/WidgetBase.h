// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void SetSelectedWidget(UUserWidget* WidgetToSelect);
	
	TArray<UUserWidget*> WidgetItems;

	uint8 SelectedWidgetItemIndex, VerticalWidgetNavigationStep, HorizontalWidgetNavigationStep;

	
public:

	virtual void SetSelectedWidget();

	UUserWidget* SelectedWidget;

	virtual void SetFocusOptions();

	virtual void Navigate_UpDown(bool bDirection);
};
