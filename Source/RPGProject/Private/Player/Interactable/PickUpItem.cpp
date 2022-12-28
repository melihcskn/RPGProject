// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interactable/PickUpItem.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Player/PlayerCharacter/Components/PlayerInventory.h"

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

void APickUpItem::Interact(UObject* InteractedObject)
{
	IInterface_InteractableItem::Interact(InteractedObject);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InteractedObject);
	if(IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetInventoryComponent()))
	{
		PlayerCharacter->AddItemToPlayerInventory(ItemID);
		Destroy();
	}
}

FString APickUpItem::GetItemName()
{
	return ItemName.ToString();
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
}

