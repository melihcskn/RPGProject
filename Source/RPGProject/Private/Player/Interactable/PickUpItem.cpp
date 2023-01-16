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

	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName("PickUpItem");
	OverlapComponent->SetCollisionProfileName("PickUpItem");
}

void APickUpItem::Interact(UObject* InteractedObject)
{
	IInterface_InteractableItem::Interact(InteractedObject);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InteractedObject);
	if(IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetInventoryComponent()))
	{
		if(ItemQuantity!=0)
		{
			PlayerCharacter->AddItemToPlayerInventory(ItemID,ItemQuantity);
		}
		else
		{
			PlayerCharacter->AddItemToPlayerInventory(ItemID);
		}
		Destroy();
	}
}

FString APickUpItem::GetItemName()
{
	return ItemName.ToString();
}

void APickUpItem::AddQuantity(float QuantityToAdd)
{
	ItemQuantity += QuantityToAdd;
}

void APickUpItem::SetQuantity(float QuantityToSet)
{
	ItemQuantity = QuantityToSet;
}

float APickUpItem::GetQuantity()
{
	return ItemQuantity;
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickUpItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	//Check if other actor is also pickupitem
	if(Cast<APickUpItem>(OtherActor))
	{
		//Check if this actor is moving
		if(GetVelocity().Size()>0.0f)
		{
			//If this actor is moving, probably this is the new one.
			//So add this items quantity to old one and destroy this
			Cast<APickUpItem>(OtherActor)->AddQuantity(ItemQuantity);
			Destroy();
		}
	}
}
