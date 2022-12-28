// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface_InteractableItem.h"
#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.generated.h"

class USphereComponent;

UCLASS()
class RPGPROJECT_API APickUpItem : public AActor, public IInterface_InteractableItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();

	virtual void Interact(UObject* InteractedObject) override;

	virtual FString GetItemName() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item ID")
	FName ItemID;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item ID")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly,Category="Components")
		UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USphereComponent* OverlapComponent;
};
