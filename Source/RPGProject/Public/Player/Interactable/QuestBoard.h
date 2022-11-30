// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestBoard.generated.h"

USTRUCT(BlueprintType)
struct FQuests
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
		FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
		int32 QuestItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
		FString QuestExplanation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
		FString QuestItem;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Struct")
		UTexture2D* QuestItemImage;

	FQuests()
	{
		QuestName = "test";
		QuestItemQuantity = 0;
		QuestExplanation = nullptr;
		QuestItem = nullptr;
		QuestItemImage=nullptr;
	}

	FQuests(FString QName, int32 ItemQuantity, FString QExplanation, FString QItem,UTexture2D* QItemImage)
	{
		QuestName = QName;
		QuestItemQuantity = ItemQuantity;
		QuestExplanation = QExplanation;
		QuestItem = QItem;
		QuestItemImage = QItemImage;
	}
};

UCLASS()
class RPGPROJECT_API AQuestBoard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Image")
		UTexture2D* RingQuestItemImage;

	UPROPERTY(EditDefaultsOnly,Category="Image")
	UTexture2D* EnemyKillQuestItemImage;

	UPROPERTY(BlueprintReadWrite)
		TArray<FQuests> QuestList;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComp;

};
