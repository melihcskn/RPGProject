// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerInput.h"
#include "MyGameInstance.generated.h"

class UMainMenu_BaseWidget;
class UInputSettings;

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
	
};
