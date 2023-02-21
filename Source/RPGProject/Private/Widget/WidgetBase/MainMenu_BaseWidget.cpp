// Fill out your copyright notice in the Description page of Project Settings.
//When navigating UP means decreasing the number of selected item index, DOWN means increasing the selected item index
#define UP false
#define DOWN true

#include "Widget/WidgetBase/MainMenu_BaseWidget.h"

#include "Widget/WidgetInterface/MenuItemsInterface.h"
#include "Widget/WidgetInterface/MenuSwitchableItemInterface.h"
#include "MyGameInstance.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "WidgetPlayerController.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu_BaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	//Pre casting
	GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	PC = Cast<AWidgetPlayerController>(UGameplayStatics::GetPlayerController(this,0));
	MyHud = Cast<APlayerHUD>(PC->GetHUD());
}

