// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interactable/ItemShop.h"

#include <activation.h>


#include "Player/Interactable/PickUpItem.h"
#include "Components/CapsuleComponent.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AItemShop::AItemShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemShop_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	ItemShop_MeshComp->SetupAttachment(RootComponent);
	ItemShop_SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	ItemShop_SphereComp->SetupAttachment(ItemShop_MeshComp);
	ItemShop_SphereComp->SetSphereRadius(85);
}

// Called when the game starts or when spawned
void AItemShop::BeginPlay()
{
	Super::BeginPlay();

	ItemShop_SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AItemShop::ItemShop_OverlapBegin);
	ItemShop_SphereComp->OnComponentEndOverlap.AddDynamic(this,&AItemShop::ItemShop_OverlapEnd);
}

// Called every frame
void AItemShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemShop::Add_ShopItem(FString NewShopItem_Name,UTexture2D* NewShopItem_Image,int32 NewShopItem_Price,int32 NewShopItem_Amount)
{
	ShopItems.Push(FShopItems(NewShopItem_Name,NewShopItem_Image,NewShopItem_Price,NewShopItem_Amount));
}

void AItemShop::ItemShop_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappedActor = Cast<APlayerCharacter>(OtherActor);
	if(OverlappedActor)
	{ShowOverlapMessage();}
}

void AItemShop::ItemShop_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedActor = nullptr;
	CloseOverlapMessage();
}

void AItemShop::TakeCoinFromPlayer(int32 ItemPrice,FString ItemName, UTexture2D* ItemTexture,int32 ItemQuantity)
{
	
	if((OverlappedActor->GetPlayerCoin() - ItemPrice)>=0 && OverlappedActor)
	{

	}
	else{ItemShopWarning();}
}


