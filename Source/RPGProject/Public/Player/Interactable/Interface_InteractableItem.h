// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_InteractableItem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_InteractableItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPROJECT_API IInterface_InteractableItem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Interact(UObject* InteractedObject);

	virtual FString GetItemName();
};
