// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interactable/QuestBoard.h"
#include "Player/Interactable/QuestDataAsset.h"
#include "MyGameModeBase.h"
#include "Widget/Widget/QuestBoardWidget.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"

// Sets default values
AQuestBoard::AQuestBoard()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComp;
	
}

// Called when the game starts or when spawned
void AQuestBoard::BeginPlay()
{
	Super::BeginPlay();

	AMyGameModeBase* MyGameModeBase = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	if(MyGameModeBase)
	{
		bool bIsFound;
		Quests.Push(MyGameModeBase->FindQuest("Quest1",bIsFound));
		Quests.Push(MyGameModeBase->FindQuest("Quest2",bIsFound));
		Quests.Push(MyGameModeBase->FindQuest("Ring Quest", bIsFound));
	}
}

void AQuestBoard::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

void AQuestBoard::Interact(UObject* InteractedObject)
{
	IInterface_InteractableItem::Interact(InteractedObject);
	
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InteractedObject))
	{
		if(QuestBoardWidgetClass)
		{
			UQuestBoardWidget* QBWidget = Cast<UQuestBoardWidget>(CreateWidget(PlayerCharacter->GetPlayerController(),QuestBoardWidgetClass));
			if(QBWidget)
			{
				QBWidget->SetQuests(Quests);
				QBWidget->SetOwner(PlayerCharacter);
				PlayerCharacter->AddWidget(QBWidget);
			}
		}
	}
}