// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestBoard_WidgetItemIcon.generated.h"

class UImage;
class UTextBlock;
class UBorder;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestBoard_WidgetItemIcon : public UUserWidget
{
	GENERATED_BODY()


protected:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemQuantityText;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

public:

	void SetInitials(UTexture2D* ImageToSet, int32 ItemQuantity);

};
