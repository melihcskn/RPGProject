// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/Components/PlayerInventory.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"

// Sets default values for this component's properties
UPlayerInventory::UPlayerInventory()
{
	MyOwner = Cast<APlayerCharacter>(GetOwner());
}


// Called when the game starts
void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();

	MaxPlayerCoin = 999999999.0f;
}

TArray<FItem> UPlayerInventory::GetInventoryItems()
{
	return InventoryItems;
}

float UPlayerInventory::GetPlayerCoin()
{
	return PlayerCoin;
}

void UPlayerInventory::AddItem(FItem ItemToAdd)
{
	//If item already exist in the inventory increase the quantity
	bool bIsItemFound;
	int32 ItemIndex = ItemIndexFinder(ItemToAdd.ItemID,bIsItemFound);
	if(bIsItemFound)
	{
		InventoryItems[ItemIndex].ItemQuantity += ItemToAdd.ItemQuantity;
	}
	else
	{
		InventoryItems.Add(ItemToAdd);
	}
}

void UPlayerInventory::AddCoin(float CoinAmountToAdd)
{
	PlayerCoin = FMath::Clamp(PlayerCoin+CoinAmountToAdd,0.0f,MaxPlayerCoin);
}

int32 UPlayerInventory::ItemIndexFinder(FName ItemIDToFind,bool& IsItemFound)
{
	IsItemFound=false;
	//Check if inventory already has the item, if found return index
	for(int i =0;i<InventoryItems.Num();i++)
	{
		if(InventoryItems[i].ItemID==ItemIDToFind)
		{
			IsItemFound=true;
			return i;
		}
	}
	return 0;
}

void UPlayerInventory::RemoveItem(FName ItemIDToRemove, int32 Quantity)
{
	bool bIsItemFound;
	int32 ItemIndex;
	ItemIndex = ItemIndexFinder(ItemIDToRemove,bIsItemFound);
	if(bIsItemFound)
	{
		InventoryItems[ItemIndex].ItemQuantity = FMath::Clamp(InventoryItems[ItemIndex].ItemQuantity-Quantity,0,InventoryItems[ItemIndex].ItemQuantity);
		if(InventoryItems[ItemIndex].ItemQuantity == 0)
		{
			InventoryItems.RemoveAt(ItemIndex);
			OnItemRemove.Broadcast();
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Item not found"));
	}
}
