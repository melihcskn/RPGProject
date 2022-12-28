// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interactable/Interface_InteractableItem.h"

// Add default functionality here for any IInterface_InteractableItem functions that are not pure virtual.
void IInterface_InteractableItem::Interact(UObject* InteractedObject)
{
}

FString IInterface_InteractableItem::GetItemName()
{
	return "";
}
