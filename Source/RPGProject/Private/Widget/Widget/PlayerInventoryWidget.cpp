// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/PlayerInventoryWidget.h"

#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Widget/WidgetItemBase/PlayerInventory_WidgetItem.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "Player/PlayerCharacter/Components/PlayerInventory.h"

void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	HorizontalWidgetNavigationStep = 1;
	VerticalWidgetNavigationStep = 4;
	SelectedWidgetItemIndex = 0;
	CreateItems();

	SetItemDropPopUpVisibility(false);

	DropItemButtonText->SetText(FText::FromName("Drop"));
	CancelDropItemButtonText->SetText(FText::FromName("Cancel"));

	DropItemButton->OnClicked.AddDynamic(this,&UPlayerInventoryWidget::DropItem);
	CancelDropItemButton->OnClicked.AddDynamic(this,&UPlayerInventoryWidget::CancelDropItem);
}

void UPlayerInventoryWidget::SwitchLeft()
{
	IMenuSwitchableItemInterface::SwitchLeft();
}

void UPlayerInventoryWidget::SwitchRight()
{
	IMenuSwitchableItemInterface::SwitchRight();
}

//Create Inventory slots and set them
void UPlayerInventoryWidget::CreateItems()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	APlayerHUD* MyMud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if(!MyMud->PlayerInventoryItemWidgetClass)
		return;
	
	if(PlayerCharacter)
		PlayerInventory = PlayerCharacter->GetInventoryComponent();

	for(int i =0 ;i<PlayerInventory->GetMaxInventoryItem();i++)
	{
		UPlayerInventory_WidgetItem* WI = Cast<UPlayerInventory_WidgetItem>(CreateWidget(this,MyMud->PlayerInventoryItemWidgetClass));
		if(WI)
		{
			WI->SetItem(FItem(),i);
			InventoryMainPanel->AddChildToWrapBox(WI);
			WidgetItems.Push(WI);
		}
	}
	
	SetItems();
}

//Set Inventory items
void UPlayerInventoryWidget::SetItems()
{
	if(PlayerInventory)
	{
		for(int i =0;i<PlayerInventory->GetInventoryItems().Num();i++)
		{
			Cast<UPlayerInventory_WidgetItem>(InventoryMainPanel->GetAllChildren()[i])->SetItem(PlayerInventory->GetInventoryItems()[i],i);
		}
	}
}

//Clear and set Inventory items
void UPlayerInventoryWidget::RefreshItems()
{
	for(int i=0;i<InventoryMainPanel->GetAllChildren().Num();i++)
	{
		Cast<UPlayerInventory_WidgetItem>(InventoryMainPanel->GetAllChildren()[i])->SetItem(FItem(),i);
	}
	
	SetItems();
}

//Set itemdrop popup visibility
void UPlayerInventoryWidget::SetItemDropPopUpVisibility(bool IsVisible)
{
	if(IsVisible)
	{
		ItemDropSpinBox->SetVisibility(ESlateVisibility::Visible);
		DropItemButton->SetVisibility(ESlateVisibility::Visible);
		CancelDropItemButton->SetVisibility(ESlateVisibility::Visible);
		DropItemButtonText->SetVisibility(ESlateVisibility::Visible);
		CancelDropItemButtonText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemDropSpinBox->SetVisibility(ESlateVisibility::Hidden);
		DropItemButton->SetVisibility(ESlateVisibility::Hidden);
		CancelDropItemButton->SetVisibility(ESlateVisibility::Hidden);
		DropItemButtonText->SetVisibility(ESlateVisibility::Hidden);
		CancelDropItemButtonText->SetVisibility(ESlateVisibility::Hidden);
	}
}

//Execute when player clicked on drop button
void UPlayerInventoryWidget::DropItem()
{
	OnItemDrop.Execute(TempItemID,ItemDropSpinBox->GetValue());
	SetItemDropPopUpVisibility(false);
	TempItemID = "";
}

//Execute when player clicked on cancel button
void UPlayerInventoryWidget::CancelDropItem()
{
	SetItemDropPopUpVisibility(false);
	TempItemID = "";
}

//Open p
void UPlayerInventoryWidget::OpenItemDropPopUp(FName ItemID)
{
	if(PlayerInventory)
	{
		bool bIsItemFound;
		int32 ItemIndex = PlayerInventory->ItemIndexFinder(ItemID,bIsItemFound);
		if(bIsItemFound)
		{
			TempItemID = ItemID;
			ItemDropSpinBox->SetMaxSliderValue(PlayerInventory->GetInventoryItems()[ItemIndex].ItemQuantity);
			ItemDropSpinBox->SetMaxValue(PlayerInventory->GetInventoryItems()[ItemIndex].ItemQuantity);
			ItemDropSpinBox->SetValue(PlayerInventory->GetInventoryItems()[ItemIndex].ItemQuantity);
			SetItemDropPopUpVisibility(true);
		}
		else
		{
			SetItemDropPopUpVisibility(false);
		}
	}
}

//Refresh Inventory Widget
void UPlayerInventoryWidget::RefreshInventory()
{
	RefreshItems();
}
