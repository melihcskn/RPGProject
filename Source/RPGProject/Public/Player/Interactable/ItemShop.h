// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Chaos/AABBTree.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ItemShop.generated.h"

class UCapsuleComponent;
class APlayerCharacter;

USTRUCT(BlueprintType)
struct FShopItems
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Struct")
	FString ItemName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Struct")
	UTexture2D* ItemImage=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Struct")
	int32 ItemPrice;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Struct")
	int32 ItemAmount;

	FShopItems()
	{
		ItemName = "Item name has not been set";
		ItemImage = nullptr;
		ItemPrice = 0;
		ItemAmount=0;
	}

	FShopItems(FString NewShopItem_Name,UTexture2D* NewShopItem_Image,int32 NewShopItem_Price,int32 NewShopItem_Amount)
	{
		ItemName=NewShopItem_Name;
		ItemImage=NewShopItem_Image;
		ItemPrice=NewShopItem_Price;
		ItemAmount=NewShopItem_Amount;
	}
};

UCLASS()
class RPGPROJECT_API AItemShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
		TArray<FShopItems> ShopItems;

	UPROPERTY(EditDefaultsOnly,Category="MeshComponent")
		UStaticMeshComponent* ItemShop_MeshComp;

	UPROPERTY(EditDefaultsOnly,Category="CollisionComponent")
		USphereComponent* ItemShop_SphereComp;
	
	UPROPERTY(BlueprintReadOnly)
		APlayerCharacter* OverlappedActor=nullptr;

	UFUNCTION(BlueprintImplementableEvent)
		void ItemShopWarning();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void ShowOverlapMessage();

	UFUNCTION(BlueprintImplementableEvent)
		void CloseOverlapMessage();
	
	UFUNCTION(BlueprintCallable)
		void Add_ShopItem(FString NewShopItem_Name,UTexture2D* NewShopItem_Image,int32 NewShopItem_Price,int32 NewShopItem_Amount);

	UFUNCTION(BlueprintCallable)
		void TakeCoinFromPlayer(int32 ItemPrice,FString ItemName, UTexture2D* ItemTexture,int32 ItemQuantity);
	
	UFUNCTION()
	void ItemShop_OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
	void ItemShop_OverlapEnd(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

};
