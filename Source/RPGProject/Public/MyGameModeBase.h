// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"


class UItemDataAsset;
struct FItem;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="ItemDatabase")
	UItemDataAsset* ItemDatabase;

public:

	FItem FindItem(FName ItemID,bool& IsItemFound);
};
