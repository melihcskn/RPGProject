// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "MySaveGame.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Player/PlayerCharacter/Components/PlayerCharacter_HealthComponent.h"
#include "Player/PlayerCharacter/Components/PlayerInventory.h"
#include "Player/PlayerCharacter/Weapon/PlayerWeapon.h"

void UMyGameInstance::Init()
{
	Super::Init();
	
	bIsGameStarted=false;
	InputSetting = UInputSettings::GetInputSettings();
}

void UMyGameInstance::SaveGame()
{
	//Create save game instance and slot
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetFirstLocalPlayerController()->GetPawn());
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if(SaveGameInstance==nullptr)return;
	//Save gun if exist
	if(PlayerCharacter->GetWeaponClass())
	{
		SaveGameInstance->PlayerWeaponClass = PlayerCharacter->GetWeaponClass();
	}
	//Save inventory items if any exist
	if(PlayerCharacter->GetInventoryComponent()->GetInventoryItems().IsValidIndex(0))
	{
		for(int i=0;i<PlayerCharacter->GetInventoryComponent()->GetInventoryItems().Num();i++)
		{
			FName ItemID = PlayerCharacter->GetInventoryComponent()->GetInventoryItems()[i].ItemID;
			int32 ItemQuantity = PlayerCharacter->GetInventoryComponent()->GetInventoryItems()[i].ItemQuantity;
			SaveGameInstance->PlayerInventory.Add(FSaveGameInventory(ItemID,ItemQuantity));
		}
	}

	//Save basic player stats
	SaveGameInstance->PlayerHealth = PlayerCharacter->GetHealthComponent()->GetCurrentHP();
	SaveGameInstance->PlayerCoin = PlayerCharacter->GetInventoryComponent()->GetPlayerCoin();
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance,"DefaultSlot",0);
}

void UMyGameInstance::LoadGame()
{
	//Load game if specified save game exist
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetFirstLocalPlayerController()->GetPawn());
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("DefaultSlot",0));
	if(LoadGameInstance==nullptr)return;
	//If weapon saved spawn it and call player character's weapon preparation function
	if(LoadGameInstance->PlayerWeaponClass)
	{
		APlayerWeapon* PW = Cast<APlayerWeapon>(GetWorld()->SpawnActor(LoadGameInstance->PlayerWeaponClass));
		PlayerCharacter->PrepareControlledWeapon(PW);
	}
	//If any inventory item saved, set inventory
	if(LoadGameInstance->PlayerInventory.IsValidIndex(0))
	{
		for(int i=0;i<LoadGameInstance->PlayerInventory.Num();i++)
		{
			PlayerCharacter->AddItemToPlayerInventory(LoadGameInstance->PlayerInventory[i].ItemID,LoadGameInstance->PlayerInventory[i].ItemQuantity);
		}
	}
}

void UMyGameInstance::SetGameState(TEnumAsByte<EGameState> State)
{
	GameState = State;
	GameStateChange.Broadcast(GameState);
}
