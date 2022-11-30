// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInventory.generated.h"

class APlayerCharacter;
USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
	FString Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
	int32 ItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
	UTexture2D* ItemImage;

	FInventory()
	{
		Item = "test";
		ItemQuantity = 0;
		ItemImage = nullptr;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UPlayerInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInventory();

protected:
	virtual void BeginPlay() override;

	APlayerCharacter* MyOwner;

	int32 FreeInventorySlotFinder();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		float InventorySizeByPiece;
	
	UFUNCTION(BlueprintCallable)
		TArray<FInventory> GetInventory();

	UFUNCTION(BlueprintCallable)
		void SetInventory(TArray<FInventory> InventoryToSet);

public:

	TArray<FInventory> Inventory;
	
	void AddItem(FString ItemName,UTexture2D* ItemTexture,int32 ItemQuantity);

	UFUNCTION(BlueprintCallable)
	int32 ItemFinder(FString ItemNameToFind);

	UFUNCTION(BlueprintCallable)
    void RemoveItem(int32 ItemIndex, int32 Quantity);

	TArray<FInventory> GetInventoryPlayerCharacter();
};
