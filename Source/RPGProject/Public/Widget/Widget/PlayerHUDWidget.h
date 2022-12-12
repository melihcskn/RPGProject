// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;

public:

	void SetInitials();

	void SetHealth(float HP);

	void SetAmmo(int32 ReaminingBulletInMag, int32 MaxMagSize);

	void SetAmmoTextHidden();

	void SetQuest();
	
	
};
