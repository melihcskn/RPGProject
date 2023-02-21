// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetItemBase/QuestBoard_WidgetItem.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UQuestBoard_WidgetItem::NativeConstruct()
{
	Super::NativeConstruct();

	ItemButton->OnClicked.AddDynamic(this, &UQuestBoard_WidgetItem::ItemButtonClicked);
	bIsFocusable =false;
	SetButtonStyles();
}

void UQuestBoard_WidgetItem::Apply()
{
	IMenuItemsInterface::Apply();
}

void UQuestBoard_WidgetItem::SetButtonStyles()
{
	ItemButton->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Box;
	ItemButton->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Box;
	ItemButton->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Box;
	
}

void UQuestBoard_WidgetItem::ItemButtonClicked()
{
	ItemClicked.Execute(GetItemIndex());
}

void UQuestBoard_WidgetItem::SetItemText(FName ItemNameToSet)
{
	ItemText->SetText(FText::FromName(ItemNameToSet));
}

void UQuestBoard_WidgetItem::SetSelected()
{
	IMenuItemsInterface::SetSelected();

	ItemBorder->SetBrushColor(FLinearColor::Red);
}

void UQuestBoard_WidgetItem::UnSelect()
{
	IMenuItemsInterface::UnSelect();

	ItemBorder->SetBrushColor(FLinearColor::White);
}
