// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface_InteractableItem.h"
#include "GameFramework/Actor.h"
#include "QuestBoard.generated.h"

struct FQuest;

UCLASS()
class RPGPROJECT_API AQuestBoard : public AActor, public IInterface_InteractableItem
{
	GENERATED_BODY()
	
	// Sets default values for this actor's properties
	AQuestBoard();

	TArray<FQuest> Quests;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:

	virtual void Interact(UObject* InteractedObject) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UUserWidget> QuestBoardWidgetClass;

};
