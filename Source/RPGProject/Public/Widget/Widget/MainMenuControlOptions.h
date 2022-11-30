// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "MainMenuControlOptions.generated.h"

class UVerticalBox;
class UScrollBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenuControlOptions : public UMainMenu_BaseWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;

	virtual void Navigate_UpDown(bool bDirection) override;

	void PrepareVerticalBox();

	uint8 SelectedItem = 0;

public:

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UVerticalBox* VB;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UScrollBox* SB;

	UPROPERTY(meta =(BindWidget))
	UTextBlock* KeyBindWarningMessage;

	void SetTextVisibility(bool Status);
};
