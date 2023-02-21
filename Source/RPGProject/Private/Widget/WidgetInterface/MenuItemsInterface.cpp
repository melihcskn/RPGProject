// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetInterface/MenuItemsInterface.h"

// Add default functionality here for any IMenuItemsInterface functions that are not pure virtual.
void IMenuItemsInterface::SetSelected()
{
}

void IMenuItemsInterface::UnSelect()
{
}

void IMenuItemsInterface::Apply()
{
}

int32 IMenuItemsInterface::GetItemIndex()
{
	return ItemIndex;
}

void IMenuItemsInterface::SetItemIndex(int32 ItemIndexToSet)
{
	ItemIndex = ItemIndexToSet;
}
