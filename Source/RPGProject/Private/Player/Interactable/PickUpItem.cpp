// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interactable/PickUpItem.h"
#include "Components/SphereComponent.h"

// Sets default values
APickUpItem::APickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = StaticMeshComp;

	OverlapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	OverlapComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
}

UTexture2D* APickUpItem::GetPickUpItemTexture()
{
	return ItemPicture;
}

FString APickUpItem::GetPickUpItemName()
{
	return SetPickupItemName;
}


