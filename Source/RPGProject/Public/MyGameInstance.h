// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerInput.h"
#include "MyGameInstance.generated.h"

class UMainMenu_BaseWidget;
class UInputSettings;

UENUM()
enum EGameState
{
	Game,
	Menu,
	Loading
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChange,TEnumAsByte<EGameState>, SetGameState);

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:

	virtual void Init() override;

public:

	UInputSettings* InputSetting;

	bool bIsGameStarted;

	void SetGameState(TEnumAsByte<EGameState> State);

	UPROPERTY()
	TEnumAsByte<EGameState> GameState;

	UPROPERTY()
	FOnGameStateChange GameStateChange;
	
};
