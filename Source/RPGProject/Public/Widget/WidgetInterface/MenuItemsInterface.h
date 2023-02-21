// Fill out your copyright notice in the Description page of Project Settings.

DECLARE_DELEGATE_OneParam(FOnItemClicked, int32 ItemIndex);

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuItemsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuItemsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPROJECT_API IMenuItemsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	int32 ItemIndex;

	virtual void SetSelected();

	virtual void UnSelect();

	virtual void Apply();

	int32 GetItemIndex();

	void SetItemIndex(int32 ItemIndexToSet);

	FOnItemClicked ItemClicked;
};
