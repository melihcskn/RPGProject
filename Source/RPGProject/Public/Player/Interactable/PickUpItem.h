// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.generated.h"

class USphereComponent;
UCLASS()
class RPGPROJECT_API APickUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();

	UTexture2D* GetPickUpItemTexture();

	FString GetPickUpItemName();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		FString ItemName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Components")
		UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USphereComponent* OverlapComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
		FString SetPickupItemName = "PickUpItem";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
		UTexture2D* ItemPicture;
};
