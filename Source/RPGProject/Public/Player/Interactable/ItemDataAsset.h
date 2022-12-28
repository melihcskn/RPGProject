// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Struct")
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Struct")
	FText ItemDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Struct")
	int32 ItemQuantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Struct")
	UTexture2D* ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Struct")
	TSubclassOf<AActor> ItemClass;

	FItem()
	{
		ItemID = "test";
		ItemDescription = FText::FromName("Test Item Description");
		ItemQuantity = 0;
		ItemIcon = nullptr;
		ItemClass=nullptr;
	}
};


/**
 * 
 */
UCLASS()
class RPGPROJECT_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item Data")
	TArray<FItem> ItemData;
	
};
