// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_BaseControls.generated.h"

class UInputKeySelector;
class UBorder;
class UButton;
class UTextBlock;
class APlayerHUD;
class UMyGameInstance;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainMenu_BaseControls : public UUserWidget, public IMenuItemsInterface
{
	GENERATED_BODY()


protected:
	
	virtual void NativeConstruct() override;

	virtual void Apply() override;

	virtual void SetSelected() override;

	virtual void UnSelect() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;


private:

	//Store the information of whether this input action or axis
	bool ItemType;

	bool bIsReadyForKeyInput;
	
	uint8 ItemIndex;

public:

	APlayerHUD* MyHud;

	UMyGameInstance* GI;

	void SetItem_Index_Type(uint8 Index, bool Type);

	bool GetItemType();

	UPROPERTY(meta =(BindWidget))
	UButton* KeyButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* KeyButtonText;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* SelectionBorder;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemText;
	
	void ReplaceKey(FKey Key);
};
