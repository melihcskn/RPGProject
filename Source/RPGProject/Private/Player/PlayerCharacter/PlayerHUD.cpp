// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/PlayerHUD.h"

#include "WidgetPlayerController.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "Engine/Canvas.h"

// void APlayerHUD::DrawHUD()
// {
// 	Super::DrawHUD();
//
// 	if (CrosshairTexture)
// 	{
// 		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
//
// 		FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));
// 		
// 		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
// 		TileItem.BlendMode = SE_BLEND_Translucent;
// 		Canvas->DrawItem(TileItem);
// 	}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerHUD::RemoveWidget()
{
	if(WidgetHistory.Num()-1>0)
	{
		WidgetHistory.Last()->RemoveFromParent();
		WidgetHistory.Last()->Destruct();
		WidgetHistory.Pop();
		CurrentWidget = WidgetHistory.Last();
		CurrentWidget->SetFocusOptions();
		CurrentWidget->SetKeyboardFocus();
	}
}
