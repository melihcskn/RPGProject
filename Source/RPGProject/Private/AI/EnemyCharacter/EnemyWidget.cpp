// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter/EnemyWidget.h"

void UEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemyWidget::Set_Health_Bar(float const health)
{
	Health_Bar->SetPercent(health);
}
