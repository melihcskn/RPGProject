// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/Interactable/ItemDataAsset.h"
#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "PlayerInventory_WidgetItem.generated.h"

class UBorder;
class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API UPlayerInventory_WidgetItem : public UUserWidget, public IMenuItemsInterface
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	void SetButtonStyle();

	uint8 ItemIndex;
	
	FItem Item;

	// UPROPERTY(meta=(BindWidget))
	// UBorder* ItemBorder;

	UPROPERTY(meta=(BindWidget))
	UButton* ItemButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemQuantityText;

	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;

public:

	void SetItem(FItem ItemToSet, uint8 SetItemIndex);

	virtual void SetSelected() override;

	virtual void UnSelect() override;

	FItem GetItem();
	
protected:
	
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
};
