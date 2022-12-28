// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT()
struct FSaveGameInventory
{
	GENERATED_BODY()

	FName ItemID;
	int32 ItemQuantity;
	FSaveGameInventory()
	{
		ItemID = "";
		ItemQuantity=0;
	}
	FSaveGameInventory(FName ItemIDToSet,int32 QuantityToSet)
	{
		ItemID = ItemIDToSet;
		ItemQuantity = QuantityToSet;
	}
};

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

private:

	UMySaveGame();

public:

	UPROPERTY()
		UClass* PlayerWeaponClass;

	UPROPERTY()
		float PlayerHealth;

	UPROPERTY()
		float PlayerCoin;

	UPROPERTY()
		TArray<FSaveGameInventory> PlayerInventory;
	
};
