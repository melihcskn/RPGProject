// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/PlayerHUD.h"

#include "WidgetPlayerController.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "Engine/Canvas.h"
#include "Player/PlayerCharacter/Components/PlayerCharacter_HealthComponent.h"
#include "Player/PlayerCharacter/Components/PlayerInventory.h"
#include "Player/PlayerCharacter/Weapon/PlayerWeapon.h"
#include "Widget/Widget/MainMenu.h"
#include "Widget/Widget/PlayerHUDWidget.h"

// void APlayerHUD::DrawHUD()
// {
// 	Super::DrawHUD();
//
// 	if (CrosshairTexture)
// 	{
// 		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
//
// 		FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));
// 		
// 		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
// 		TileItem.BlendMode = SE_BLEND_Translucent;
// 		Canvas->DrawItem(TileItem);
// 	}

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
		OwnerInventory->OnItemRemove.AddDynamic(this,&APlayerHUD::RefeshOnItemChange);

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
			MMenu->AddToViewport();
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
		
	}
}

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
		PlayerHUD->SetAmmoTextHidden();
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
					PlayerHUD->SetInitials();
					PlayerHUD->AddToViewport();
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

void APlayerHUD::RemoveWidget()
{
	if(WidgetHistory.Num()-1>0)
	{
		WidgetHistory.Last()->RemoveFromParent();
		WidgetHistory.Last()->Destruct();
		WidgetHistory.Pop();
		CurrentWidget = WidgetHistory.Last();
		CurrentWidget->SetFocusOptions();
		CurrentWidget->SetKeyboardFocus();
	}
}
