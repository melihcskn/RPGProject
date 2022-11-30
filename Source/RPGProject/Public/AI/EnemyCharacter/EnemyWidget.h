// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void Set_Health_Bar(float const health);

protected:
	
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UProgressBar* Health_Bar;
};
