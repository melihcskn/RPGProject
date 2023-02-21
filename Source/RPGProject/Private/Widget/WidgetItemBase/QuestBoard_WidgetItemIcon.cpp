// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetItemBase/QuestBoard_WidgetItemIcon.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UQuestBoard_WidgetItemIcon::SetInitials(UTexture2D* ImageToSet, int32 ItemQuantity)
{
	ItemImage->SetBrushFromTexture(ImageToSet);
	ItemQuantityText->SetText(FText::AsNumber(ItemQuantity));
}
