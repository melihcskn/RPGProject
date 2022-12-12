// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/PlayerHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetAmmoTextHidden();
}

void UPlayerHUDWidget::SetInitials()
{
	HealthBar->BarFillType = EProgressBarFillType::BottomToTop;
	HealthBar->FillColorAndOpacity = FLinearColor(FColor::Red);
}

void UPlayerHUDWidget::SetHealth(float HP)
{
	HealthBar->SetPercent(HP/100);
	HealthText->SetText(FText::AsNumber(FMath::RoundToInt(HP)));
}

void UPlayerHUDWidget::SetAmmo(int32 RemainingBulletInMag, int32 MaxMagSize)
{
	AmmoText->SetVisibility(ESlateVisibility::Visible);
	FString AmmoString = FString::FromInt(RemainingBulletInMag).Append(" / ").Append(FString::FromInt(MaxMagSize));
	AmmoText->SetText(FText::FromString(AmmoString));
}

void UPlayerHUDWidget::SetAmmoTextHidden()
{
	AmmoText->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHUDWidget::SetQuest()
{
}
