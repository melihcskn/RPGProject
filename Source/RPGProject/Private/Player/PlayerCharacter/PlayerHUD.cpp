// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/PlayerHUD.h"

#include "WidgetPlayerController.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "GameFramework/InputSettings.h"
#include "Logging/LogMacros.h"
#include "Logging/LogMacros.h"
#include "Logging/LogMacros.h"
#include "Player/PlayerCharacter/Components/PlayerCharacter_HealthComponent.h"
#include "Player/PlayerCharacter/Components/PlayerInventory.h"
#include "Player/PlayerCharacter/Weapon/PlayerWeapon.h"
#include "Widget/Widget/MainMenu.h"
#include "Widget/Widget/PlayerHUDWidget.h"
#include "Widget/Widget/PlayerInventoryWidget.h"
#include "Widget/WidgetBase/WidgetBase.h"

void APlayerHUD::BeginPlay()
{
	Owner = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	if(Owner)
	{
		OwnerInventory = Owner->GetInventoryComponent();
		OwnerHealth = Owner->GetHealthComponent();
		Owner->OnWeaponEquipped.AddDynamic(this,&APlayerHUD::RefreshOnWeaponChange);
		Owner->OnCharacterFired.AddDynamic(this,&APlayerHUD::RefeshOnItemChange);
	}
	
	if(OwnerInventory)
		OwnerInventory->OnInventoryChange.AddUObject(this,&APlayerHUD::RefeshOnItemChange);

	if(OwnerHealth)
		OwnerHealth->OnActorDamaged.AddDynamic(this,&APlayerHUD::RefreshOnHealthChange);

	if(GI)
		GI->GameStateChange.AddDynamic(this,&APlayerHUD::RefreshOnGameStateChange);
	
	Super::BeginPlay();

	/*Prevent creating main menu once the game started
	 *So if PlayerHUD is destroyed and rebuilds main menu won't be created as default*/
	if(MainMenuClass && !GI->bIsGameStarted)
	{
		UMainMenu* MMenu = Cast<UMainMenu>(CreateWidget(GetWorld()->GetFirstPlayerController(),MainMenuClass));
		if(MMenu)
		{
			AddWidget(MMenu);
		}
	}
	
	Health=OwnerHealth->GetCurrentHP();
}

//Set widget hp bar
void APlayerHUD::SetHealth()
{
	if(Health > OwnerHealth->GetCurrentHP())
	{
		Health = FMath::Clamp(Health-HealthChangeRate,static_cast<float>(OwnerHealth->GetCurrentHP()),OwnerHealth->GetDefaultHP());
		PlayerHUD->SetHealth(Health);
	}
	else if(Health == OwnerHealth->GetCurrentHP())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

//Drop selected item
void APlayerHUD::DropItem(FName ItemID,int32 ItemQuantity)
{
	Owner->DropItemFromPlayerInventory(ItemID,ItemQuantity);
}

//When PlayerHUD gets 
void APlayerHUD::OnItemDropDetect(FName ItemID)
{
	if(UPlayerInventoryWidget* IW = Cast<UPlayerInventoryWidget>(CurrentWidget))
	{
		IW->OnItemDrop.BindUObject(this,&APlayerHUD::DropItem);
		IW->OpenItemDropPopUp(ItemID);
	}
}

//Refresh the Hud when player health change
void APlayerHUD::RefreshOnHealthChange(float Damage)
{
	if(PlayerHUD)
	{
		//Set decrease/increase rate to make hp bar change smoothly
		HealthChangeRate = Damage/180;
		//Set Widget Hp bar in time instead of instantly
		GetWorldTimerManager().SetTimer(TimerHandle,this,&APlayerHUD::SetHealth,0.001f,true,0.0f);
	}
}

//Refresh HUD elements when any HUD related item is changed
void APlayerHUD::RefeshOnItemChange()
{
	if(PlayerHUD)
	{
		if(OwnerWeapon)
		{
			PlayerHUD->SetAmmo(OwnerWeapon->GetRemainBulletInMag(),OwnerWeapon->GetMaximumMagazine());
		}
		if(UPlayerInventoryWidget* IW = Cast<UPlayerInventoryWidget>(CurrentWidget))
		{
			IW->RefreshInventory();
		}
	}
}

//Refresh HUD when player changes or equip weapon
void APlayerHUD::RefreshOnWeaponChange(bool bIsEquipped, APlayerWeapon* PlayerWeapon)
{
	if(bIsEquipped)
	{
		//Show ammo when player equip a weapon
		OwnerWeapon = PlayerWeapon;
		RefeshOnItemChange();
	}
	else
	{
		//Hide if player drops or unequip weapon
		PlayerHUD->SetAmmo(0,0);
	}
}

//Hide PlayerHUD widget when game state is not game, show when game continues
void APlayerHUD::RefreshOnGameStateChange(TEnumAsByte<EGameState> State)
{
	if(State == EGameState::Game)
	{
		if(PlayerHUD)
		{
			PlayerHUD->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			if(PlayerHUDWidgetClass)
			{
				PlayerHUD = Cast<UPlayerHUDWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(),PlayerHUDWidgetClass));
				if(PlayerHUD)
				{
					PlayerHUD->OnItemDrop.BindUObject(this,&APlayerHUD::OnItemDropDetect);
					PlayerHUD->SetInitials();
					PlayerHUD->AddToViewport();
					RefeshOnItemChange();
				}
			}
		}
	}
	else
	{
		if(PlayerHUD)
			PlayerHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

//Set interaction message
void APlayerHUD::SetInteract(ESlateVisibility VisibilityOption, FString ItemName)
{
	if(PlayerHUD && VisibilityOption == ESlateVisibility::Visible)
	{
		TArray<FInputActionKeyMapping> KMapping;
		GI->InputSetting->GetActionMappingByName("PickUp",KMapping);
		FString StringToSet = FString("Press ").Append(KMapping[0].Key.GetDisplayName().ToString()).Append(" To Pick Up ").Append(ItemName);
		PlayerHUD->SetInteractionTextVisibility(VisibilityOption,StringToSet);
	}
}

//Set interaction message visibility
void APlayerHUD::SetInteract(ESlateVisibility VisibilityOption)
{
	if(PlayerHUD)
	{
		PlayerHUD->SetInteractionTextVisibility(VisibilityOption,"");
	}
}

//Remove current widget
void APlayerHUD::RemoveWidget()
{
	//Check if widgets are stacked
	if(WidgetHistory.Num()>1)
	{
		WidgetHistory.Pop();
		CurrentWidget->RemoveFromParent();
		CurrentWidget = Cast<UWidgetBase>(WidgetHistory.Last());
		CurrentWidget->SetFocusOptions();
		CurrentWidget->SetKeyboardFocus();
	}
	else if(WidgetHistory.Num() == 1)
	{
		WidgetHistory.Pop();
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		Cast<AWidgetPlayerController>(GetWorld()->GetFirstPlayerController())->SetShowMouseCursor(false);
	}
}

//Add new widget to WidgetHistory
void APlayerHUD::AddWidget(UUserWidget* WidgetToAdd)
{
	//If player tries to open same widget, remove it
	if(CurrentWidget && CurrentWidget->GetClass() == WidgetToAdd->GetClass())
	{
		RemoveWidget();
	}
	//Add new widget
	else
	{
		Cast<AWidgetPlayerController>(GetWorld()->GetFirstPlayerController())->SetShowMouseCursor(true);
		WidgetHistory.Push(WidgetToAdd);
		CurrentWidget = Cast<UWidgetBase>(WidgetToAdd);
		CurrentWidget->AddToViewport();
		CurrentWidget->SetFocusOptions();
	}
}
