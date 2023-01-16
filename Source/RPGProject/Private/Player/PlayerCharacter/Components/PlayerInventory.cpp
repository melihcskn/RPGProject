// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/Components/PlayerInventory.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"

// Sets default values for this component's properties
UPlayerInventory::UPlayerInventory()
{
	MyOwner = Cast<APlayerCharacter>(GetOwner());
	MaxInventoryItem = 16;
}


// Called when the game starts
void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();

	CurrentNumberOfItems = InventoryItems.Num();
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
		if(CurrentNumberOfItems != MaxInventoryItem)
		{
			InventoryItems.Add(ItemToAdd);
			++CurrentNumberOfItems;
			OnInventoryChange.Broadcast();
		}
		else
		{
			//TODO pop inventory is full message
		}
	}
}

void UPlayerInventory::AddCoin(float CoinAmountToAdd)
{
	PlayerCoin = FMath::Clamp(PlayerCoin+CoinAmountToAdd,0.0f,MaxPlayerCoin);
}

float UPlayerInventory::GetItemNumber()
{
	return CurrentNumberOfItems;
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

float UPlayerInventory::GetMaxInventoryItem()
{
	return MaxInventoryItem;
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
			--CurrentNumberOfItems;
		}
		OnInventoryChange.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Item not found"));
	}
}
