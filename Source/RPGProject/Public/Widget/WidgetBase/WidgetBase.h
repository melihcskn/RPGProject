// Fill out your copyright notice in the Description page of Project Settings.



#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

class APlayerHUD;
class AWidgetPlayerController;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void SetSelectedWidget(UUserWidget* WidgetToSelect);

	virtual void SetSelectedWidget();

	virtual void AddWidgetItem(UUserWidget* WidgetItemToAdd, int32 WidgetItemIndex);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void CheckPressedKeyBehaviour(FKey PressedKey);

	AWidgetPlayerController* GetPlayerController();

	APlayerHUD* GetPlayerHUD();

	UPROPERTY()
	TArray<UUserWidget*> WidgetItems;

	uint8 SelectedWidgetItemIndex, VerticalWidgetNavigationStep, HorizontalWidgetNavigationStep;

	bool bCanBeRemoved;

	
public:

	virtual void OnItemClicked(int32 ItemIndex);

	virtual void SetFocusOptions();

	virtual void Navigate_UpDown(bool bDirection);

	UPROPERTY()
	UUserWidget* SelectedWidget;
};
