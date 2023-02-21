// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget/QuestBoardWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "Player/Interactable/QuestDataAsset.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Widget/WidgetItemBase/QuestBoard_WidgetItem.h"
#include "Widget/WidgetItemBase/QuestBoard_WidgetItemIcon.h"

void UQuestBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuestItemIconsWrapBox->SetHorizontalAlignment(HAlign_Center);
	RewardItemIconsWrapBox->SetHorizontalAlignment(HAlign_Center);

	//Bind buttons
	AcceptQuestButton->OnClicked.AddDynamic(this, &UQuestBoardWidget::AcceptQuest);
	FinishQuestButton->OnClicked.AddDynamic(this, &UQuestBoardWidget::FinishQuest);
	ExitQuestBoardButton->OnClicked.AddDynamic(this, &UQuestBoardWidget::ExitQuestBoard);

	SetSelectedWidget(0);
}

void UQuestBoardWidget::OnItemClicked(int32 ItemIndex)
{
	Super::OnItemClicked(ItemIndex);

	//Select widget when mouse button clicked on it
	SetSelectedWidget(WidgetItems[ItemIndex]);
}

void UQuestBoardWidget::SetSelectedWidget(UUserWidget* WidgetToSelect)
{
	Super::SetSelectedWidget(WidgetToSelect);

	if(WidgetItems.Num()>0)
	{
		//When switching quest, change general informations
		SetQuestDetails();
	}
}

//Initialize Quest Tab
void UQuestBoardWidget::CreateQuestTabs()
{
	//If Class is valid create and add to VerticalBox
	if(QuestBoardWidgetItemClass)
	{
		for(int i =0;i<Quests.Num();i++)
		{
			UQuestBoard_WidgetItem* QBWItem = Cast<UQuestBoard_WidgetItem>(CreateWidget(this,QuestBoardWidgetItemClass));
			if(QBWItem)
			{
				QBWItem->SetItemText(Quests[i].QuestID);
				QuestsIDVerticalBox->AddChildToVerticalBox(QBWItem);
				AddWidgetItem(QBWItem,i);
			}
		}
	}
}

//Set selected quest informations
void UQuestBoardWidget::SetQuestDetails()
{
	if(Quests.Num()>0)
	{
		QuestDescriptionText->SetText(FText::FromName(Quests[SelectedWidgetItemIndex].QuestID));
		QuestExplanationText->SetText(Quests[SelectedWidgetItemIndex].QuestDescription);
	}

	//Set Accept and Finish button according to player characters quest component
	if(Owner.IsValid() && Quests.Num()>0)
	{
		if(Owner->IsQuestTaken(Quests[SelectedWidgetItemIndex].QuestID))
		{
			AcceptQuestButton->SetIsEnabled(false);
			FinishQuestButton->SetIsEnabled(true);
		}
		else
		{
			AcceptQuestButton->SetIsEnabled(true);
			FinishQuestButton->SetIsEnabled(false);
		}
	}

	//Set required quest items icon
	if(QuestBoardWidgetQuestItemIconClass)
	{
		QuestItemIconsWrapBox->ClearChildren();
		for(int i = 0; i<Quests[SelectedWidgetItemIndex].QuestItems.Num();i++)
		{
			if(Quests[SelectedWidgetItemIndex].QuestItems[i].QuestItemIcon)
			{
				UQuestBoard_WidgetItemIcon* QBWItemIcon = Cast<UQuestBoard_WidgetItemIcon>(CreateWidget(this,QuestBoardWidgetQuestItemIconClass));
				if(QBWItemIcon)
				{
					QBWItemIcon->SetInitials(Quests[SelectedWidgetItemIndex].QuestItems[i].QuestItemIcon,Quests[SelectedWidgetItemIndex].QuestItems[i].QuestItemQuantity);
					QuestItemIconsWrapBox->AddChildToWrapBox(QBWItemIcon);
				}
			}
		}
	}

	if(QuestBoardWidgetQuestItemIconClass)
	{
		RewardItemIconsWrapBox->ClearChildren();
		for(int i = 0; i<Quests[SelectedWidgetItemIndex].Rewards.Num();i++)
		{
			if(Quests[SelectedWidgetItemIndex].Rewards[i].QuestItemIcon)
			{
				UQuestBoard_WidgetItemIcon* QBWItemIcon = Cast<UQuestBoard_WidgetItemIcon>(CreateWidget(this,QuestBoardWidgetQuestItemIconClass));
				if(QBWItemIcon)
				{
					QBWItemIcon->SetInitials(Quests[SelectedWidgetItemIndex].Rewards[i].QuestItemIcon,Quests[SelectedWidgetItemIndex].Rewards[i].QuestItemQuantity);
					RewardItemIconsWrapBox->AddChildToWrapBox(QBWItemIcon);
				}
			}
		}
	}
}

//Close quest board widget
void UQuestBoardWidget::ExitQuestBoard()
{
	RemoveFromParent();
}

//Accept selected quest
void UQuestBoardWidget::AcceptQuest()
{
	if(Owner.IsValid())
	{
		Owner->AddQuest(Quests[SelectedWidgetItemIndex].QuestID);
		SetQuestDetails();
		UE_LOG(LogTemp,Warning,TEXT("SelectedWidgetItemIndex : %d"),SelectedWidgetItemIndex);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Invalid Owner"));
}

//Finish selected quest
void UQuestBoardWidget::FinishQuest()
{
	if(Owner.IsValid())
	{
		Owner->FinishQuest(Quests[SelectedWidgetItemIndex].QuestID);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Invalid Owner"));
}

//Set quests
void UQuestBoardWidget::SetQuests(TArray<FQuest> QuestsToSet)
{
	Quests = QuestsToSet;
	
	if(Quests.Num()>0)
		CreateQuestTabs();
}
