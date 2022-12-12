// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInstance.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UMyGameInstance;
class UMainMenu_BaseWidget;
class UProgressBar;
class UTextBlock;
class APlayerCharacter;
class UPlayerInventory;
class UPlayerCharacter_HealthComponent;
class UPlayerHUDWidget;
class APlayerWeapon;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	APlayerCharacter* Owner;

	UPlayerInventory* OwnerInventory;

	UPlayerCharacter_HealthComponent* OwnerHealth;

	UPlayerHUDWidget* PlayerHUD;

	APlayerWeapon* OwnerWeapon;

	UMyGameInstance* GI;

	FTimerHandle TimerHandle;

	float Health;
	
	float HealthChangeRate;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;

	void SetHealth();

	UFUNCTION()
	void RefreshOnHealthChange(float Damage);

	UFUNCTION()
	void RefeshOnItemChange();

	UFUNCTION()
	void RefreshOnWeaponChange(bool bIsEquipped, APlayerWeapon* PlayerWeapon);

	UFUNCTION()
	void RefreshOnGameStateChange(TEnumAsByte<EGameState> State);


public:

	// virtual void DrawHUD() override;

	UMainMenu_BaseWidget* CurrentWidget;

	TArray<UMainMenu_BaseWidget*> WidgetHistory;
	
	void RemoveWidget();
	
	static FVector2D CrossHairDot;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MenuButtonClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OptionsMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SoundOptionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SoundSliderClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GraphicOptionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OptionSelectionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ContrlOptionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ControlItemClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
};
