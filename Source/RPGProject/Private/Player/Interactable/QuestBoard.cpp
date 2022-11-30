// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interactable/QuestBoard.h"

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
	
	QuestList.Push(FQuests("RingQuest",1,"Find the one ring and bring it back","The One Ring",RingQuestItemImage));
	QuestList.Push(FQuests("KillQuest",10,"Kill 10 enemy","AI Head",EnemyKillQuestItemImage));
}