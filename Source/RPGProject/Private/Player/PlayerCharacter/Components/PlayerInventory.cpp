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
}

void UPlayerInventory::AddItem(FString ItemName, UTexture2D* ItemTexture,int32 ItemQuantity)
{
	int32 Index = ItemFinder(ItemName);
	int32 FreeInventorySlot = FreeInventorySlotFinder();
	if (Index != 999 && FreeInventorySlot == 999)
	{
		Inventory[Index].ItemQuantity +=ItemQuantity;
		MyOwner->RefreshInventory();
	}
	else if(FreeInventorySlot != 999)
	{
		Inventory[FreeInventorySlot].ItemImage = ItemTexture;
		Inventory[FreeInventorySlot].Item = ItemName;
		Inventory[FreeInventorySlot].ItemQuantity = ItemQuantity;
		MyOwner->RefreshInventory();
	}
	else
	{
		Inventory.Push(FInventory());
		Inventory[Inventory.Num() - 1].ItemImage = ItemTexture;
		Inventory[Inventory.Num() - 1].Item = ItemName;
		Inventory[Inventory.Num() - 1].ItemQuantity = ItemQuantity;
		MyOwner->RefreshInventory();
	}
}

int32 UPlayerInventory::FreeInventorySlotFinder()
{
	if(Inventory.IsValidIndex(0))return 999;
	if (Inventory.Num() > 0)
	{
		for (int i = 0;i < Inventory.Num(); i++)
		{
			if(Inventory[i].ItemQuantity == 0)
			{
				return i;
			}
		}
	}
	return 999;
}

TArray<FInventory> UPlayerInventory::GetInventory()
{
	return Inventory;
}

void UPlayerInventory::SetInventory(TArray<FInventory> InventoryToSet)
{
	Inventory = InventoryToSet;
}

int32 UPlayerInventory::ItemFinder(FString ItemNameToFind)
{
	if(Inventory.Num()>0)//Check inventory has any item
	{
		for (int i = 0;i < Inventory.Num(); i++)
		{
			if (Inventory[i].Item.Compare(ItemNameToFind) == 0)
			{
				return i;
			}
		}
	}
	return 999;
}

void UPlayerInventory::RemoveItem(int32 ItemIndex, int32 Quantity)
{
	Inventory[ItemIndex].ItemQuantity -= Quantity;
	if (Inventory[ItemIndex].ItemQuantity <= 0)
	{
		Inventory.RemoveAt(ItemIndex);
	}
	OnItemRemove.Broadcast();
	MyOwner->RefreshInventory();
}

TArray<FInventory> UPlayerInventory::GetInventoryPlayerCharacter()
{
	return Inventory;
}
