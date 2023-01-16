// Fill out your copyright notice in the Description page of Project Settings.

DECLARE_DELEGATE_OneParam(FOnItemDropped,FName ItemID);

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UHorizontalBox;
class UButton;
class USpinBox;
struct FItem;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	virtual void NativeConstruct() override;

public:

	void SetInitials();

	void SetHealth(float HP);

	void SetAmmo(int32 ReaminingBulletInMag, int32 MaxMagSize);

	void SetInteractionTextVisibility(ESlateVisibility VisibilityOption, FString StringToSet);

	void SetQuest();

	FOnItemDropped OnItemDrop;
	
protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;

	UPROPERTY(meta =(BindWidget))
	UTextBlock* InteractionText;

};
