// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WidgetPlayerController.generated.h"

class UMyGameInstance;
class APlayerHUD;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AWidgetPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	UMyGameInstance* GI;

	APlayerHUD* MyHud;

	UUserWidget* CurrentWidget;

	APlayerCharacter* PossessedPawn;

	UFUNCTION()
	void OpenInventory();


public:

	UFUNCTION()
	void NavigateRight();

	UFUNCTION()
	void NavigateLeft();

	UFUNCTION()
	void NavigateDown();

	UFUNCTION()
	void NavigateUp();

	UFUNCTION()
	void Apply();

	UFUNCTION()
	void Back();

	//Defining widget control keys as string
	//Since player can use both gamepad and keyboard variables are TArray
	TArray<FString> Widget_Up;
	TArray<FString> Widget_Down;
	TArray<FString> Widget_Back;
	TArray<FString> Widget_Apply;
	TArray<FString> Widget_Left;
	TArray<FString> Widget_Right;
	
};
