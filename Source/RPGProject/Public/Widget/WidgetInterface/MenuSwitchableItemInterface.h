// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuSwitchableItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuSwitchableItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPROJECT_API IMenuSwitchableItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SwitchLeft();

	virtual void SwitchRight();
};
