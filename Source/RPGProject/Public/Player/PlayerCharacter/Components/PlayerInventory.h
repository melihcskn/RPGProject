// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Interactable/ItemDataAsset.h"
#include "Components/ActorComponent.h"
#include "PlayerInventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemRemove);

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UPlayerInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInventory();

protected:
	virtual void BeginPlay() override;

	TArray<FItem> InventoryItems;

	float PlayerCoin;

	float MaxPlayerCoin;

	APlayerCharacter* MyOwner;
	
	int32 FreeInventorySlotFinder();
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		float InventorySizeByPiece;

public:

	void AddItem(FItem ItemToAdd);

	void AddCoin(float CoinAmountToAdd);

	int32 ItemIndexFinder(FName ItemIDToFind, bool& IsItemFound);

	UFUNCTION(BlueprintCallable)
	TArray<FItem> GetInventoryItems();

	float GetPlayerCoin();

	UPROPERTY()
	FOnItemRemove OnItemRemove;

	UFUNCTION(BlueprintCallable)
    void RemoveItem(FName ItemIDToRemove, int32 Quantity);
};
