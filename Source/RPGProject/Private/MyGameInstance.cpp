// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "GameFramework/InputSettings.h"

void UMyGameInstance::Init()
{
	Super::Init();

	bIsGameStarted=false;
	InputSetting = UInputSettings::GetInputSettings();
}

void UMyGameInstance::SetGameState(TEnumAsByte<EGameState> State)
{
	GameState = State;
	GameStateChange.Broadcast(GameState);
}
