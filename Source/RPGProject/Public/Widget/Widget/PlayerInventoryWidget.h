// Fill out your copyright notice in the Description page of Project Settings.

DECLARE_DELEGATE_TwoParams(FOnItemDrop, FName ItemID, int32 ItemQuantity);

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/WidgetBase/WidgetBase.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"
#include "PlayerInventoryWidget.generated.h"

class APlayerCharacter;
class UWrapBox;
class UTextBlock;
class UPlayerInventory;
class UPlayerInventory_WidgetItem;
class USpinBox;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UPlayerInventoryWidget : public UWidgetBase, public IMenuSwitchableItemInterface
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void SwitchLeft() override;

	virtual void SwitchRight() override;

	void CreateItems();

	void SetItems();

	void RefreshItems();

	void SetItemDropPopUpVisibility(bool IsVisible);

	FName TempItemID;

	int32 PanelHeight,PanelWidth;

	UPlayerInventory* PlayerInventory;

	UPROPERTY(meta =(BindWidget))
	UWrapBox* InventoryMainPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCoinText;

	UPROPERTY(meta=(BindWidget))
	USpinBox* ItemDropSpinBox;

	UPROPERTY(meta=(BindWidget))
	UButton* DropItemButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CancelDropItemButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DropItemButtonText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CancelDropItemButtonText;

	UFUNCTION()
	void DropItem();

	UFUNCTION()
	void CancelDropItem();

public:

	FOnItemDrop OnItemDrop;
	
	void OpenItemDropPopUp(FName ItemID);

	void RefreshInventory();
};
