// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/PlayerHUDWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Widget/WidgetItemBase/PlayerInventory_WidgetItem.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

//Set initial variables
void UPlayerHUDWidget::SetInitials()
{
	InteractionText->SetVisibility(ESlateVisibility::Hidden);
	HealthBar->BarFillType = EProgressBarFillType::BottomToTop;
	HealthBar->FillColorAndOpacity = FLinearColor(FColor::Red);
}

//Set player health
void UPlayerHUDWidget::SetHealth(float HP)
{
	HealthBar->SetPercent(HP/100);
	HealthText->SetText(FText::AsNumber(FMath::RoundToInt(HP)));
}

//Set ammunition text
void UPlayerHUDWidget::SetAmmo(int32 RemainingBulletInMag, int32 MaxMagSize)
{
	if(MaxMagSize == 0)
	{
		AmmoText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	AmmoText->SetVisibility(ESlateVisibility::Visible);
	FString AmmoString = FString::FromInt(RemainingBulletInMag).Append(" / ").Append(FString::FromInt(MaxMagSize));
	AmmoText->SetText(FText::FromString(AmmoString));
}

//Set interaction text visibility
void UPlayerHUDWidget::SetInteractionTextVisibility(ESlateVisibility VisibilityOption, FString StringToSet)
{
	InteractionText->SetText(FText::FromString(StringToSet));
	InteractionText->SetVisibility(VisibilityOption);
}

//TODO add quest to HUD widget
void UPlayerHUDWidget::SetQuest()
{
}

bool UPlayerHUDWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	//Check if dragvisual exist
	if(Cast<UPlayerInventory_WidgetItem>(InOperation->DefaultDragVisual))
	{
		OnItemDrop.Execute(Cast<UPlayerInventory_WidgetItem>(InOperation->DefaultDragVisual)->GetItem().ItemID);
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
