// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetItemBase/PlayerInventory_WidgetItem.h"

#include "Blueprint/DragDropOperation.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacter/PlayerHUD.h"

void UPlayerInventory_WidgetItem::NativeConstruct()
{
	Super::NativeConstruct();

	SetButtonStyle();
}

void UPlayerInventory_WidgetItem::SetButtonStyle()
{
	FSlateBrush Normal,Hovered,Pressed;

	Normal.TintColor = FSlateColor(FLinearColor(FVector(0.0f,0.0f,0.0f)));
	
	
	FButtonStyle ItemButtonStyle;
	ItemButtonStyle.Normal = Normal;
	ItemButtonStyle.Hovered = Hovered;
	ItemButtonStyle.Pressed = Pressed;

	
	ItemButton->SetStyle(ItemButtonStyle);
}

//Set item
void UPlayerInventory_WidgetItem::SetItem(FItem ItemToSet, uint8 SetItemIndex)
{
	Item = ItemToSet;
	ItemIndex = SetItemIndex;
	
	if(ItemToSet.ItemQuantity > 0)
	{
		ItemQuantityText->SetVisibility(ESlateVisibility::Visible);
		ItemQuantityText->SetText(FText::AsNumber(Item.ItemQuantity));
	}
	else
		ItemQuantityText->SetVisibility(ESlateVisibility::Hidden);

	if(Item.ItemIcon == nullptr)
	{
		ItemIcon->SetBrushFromTexture(nullptr);
	}
	else
	{
		ItemIcon->SetBrushFromTexture(Item.ItemIcon);
	}
	
	UnSelect();
}

void UPlayerInventory_WidgetItem::SetSelected()
{
	IMenuItemsInterface::SetSelected();
}

void UPlayerInventory_WidgetItem::UnSelect()
{
	IMenuItemsInterface::UnSelect();
}

FItem UPlayerInventory_WidgetItem::GetItem()
{
	return Item;
}

//Check if drag detect
FReply UPlayerInventory_WidgetItem::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
                                                                   const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Reply.DetectDrag(TakeWidget(),EKeys::LeftMouseButton);
		return Reply;
	}
	return Reply;
}

//Set drag operation
void UPlayerInventory_WidgetItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	//Create a Inventory Widget Item for dragdrop operation to handle its DragVisual
	UPlayerInventory_WidgetItem* WidgetItemVisual = Cast<UPlayerInventory_WidgetItem>(CreateWidget(GetWorld()->GetFirstPlayerController(),Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->PlayerInventoryItemWidgetClass));
	WidgetItemVisual->SetItem(Item,0);
	
	// Create DragDropOperation and set its payload and default drag visual
	OutOperation = NewObject<UDragDropOperation>();
	OutOperation->Payload = this;
	OutOperation->DefaultDragVisual = WidgetItemVisual;
	OutOperation->Pivot = EDragPivot::TopLeft;
	
	
}
