// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/PlayerCharacter.h"

#include <memory>

#include "DrawDebugHelpers.h"
#include "AI/EnemyCharacter/EnemyCharacter.h"
#include "MyGameInstance.h"
#include "Player/Interactable/PickUpItem.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/PlayerCharacter/Weapon/PlayerWeapon.h"
#include "Player/PlayerCharacter/Components/PlayerQuests.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Player/PlayerCharacter/Components/PlayerCharacter_HealthComponent.h"
#include "Player/PlayerCharacter/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter/Components/PlayerInventory.h"
#include "WidgetPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericPlatform/GenericApplication.h"
#include "Perception/AISense_Hearing.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Widget/Widget/MainMenu.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = true;

	ADSCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ADSCameraComp"));
	ADSCameraComp->SetRelativeRotation(FRotator(90,180,180));
	ADSCameraComp->SetRelativeScale3D(FVector(0.1f));
	
	PlayerHealth = CreateDefaultSubobject<UPlayerCharacter_HealthComponent>(TEXT("HealthComponent"));
	PlayerInventory = CreateDefaultSubobject<UPlayerInventory>(TEXT("InventoryComponent"));
	PlayerQuests = CreateDefaultSubobject<UPlayerQuests>(TEXT("QuestComponent"));

	WeaponSocketName = "WeaponSocket";
	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	this->OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	this->OnActorEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	PlayerHealth->OnActorKilled.AddDynamic(this,&APlayerCharacter::APlayerCharacter::GetCoinByKill);
	PlayerCoin = 500;
	RecoilCurve = 0.1f;
	WalkSpeed = 300.0f;
	RunSpeed = 600.0f;
	QParams = FCollisionQueryParams::DefaultQueryParam;
	QParams.AddIgnoredActor(this);
	QParams.bReturnPhysicalMaterial = true;
	QParams.bTraceComplex = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (ControlledWeapon)
	{
		PrepareControlledWeapon(ControlledWeapon);
	}
	LastWeaponFire = GetWorld()->TimeSeconds;

	PlayerCont=UGameplayStatics::GetPlayerController(GetWorld(),0);
	EnableMoving();

	PC = Cast<AWidgetPlayerController>(UGameplayStatics::GetPlayerController(this,0));
	GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	MyPlayerHUD = Cast<APlayerHUD>(PC->GetHUD());
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsFiring && PlayerCont)
	{
		PlayerCont->AddPitchInput(-ControlledWeapon->GetWeaponPitchRecoil()*RecoilCurve);
		PlayerCont->AddYawInput(FMath::RandRange(-ControlledWeapon->GetWeaponYawRecoil(),ControlledWeapon->GetWeaponYawRecoil()));
	}
	
		ApplyFallDamage();//TODO Move apply fall damage out of tick function
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComp = PlayerInputComponent;
	
	SetPlayerInputs();
	
}

//Get remaining bullet in mag
uint8 APlayerCharacter::GetWeaponAmmo()
{
	if (ControlledWeapon != nullptr)
	{
		return ControlledWeapon->GetRemainBulletInMag();
	}
	return 0;
}

//Get maximum magazine size of current weapon
uint8 APlayerCharacter::GetMaxMagSize()
{
	if (ControlledWeapon != nullptr)
	{
		return ControlledWeapon->GetMaximumMagazine();
	}
	return 0;
}

//Get current ammunition on player
int32 APlayerCharacter::GetCurrentAmmo()
{
	int32 AmmoIndex = PlayerInventory->ItemFinder("Ammunition");
	if(AmmoIndex==999) return 0;
	return PlayerInventory->Inventory[AmmoIndex].ItemQuantity;
}

//Open main menu
void APlayerCharacter::MainMenu()
{
	if(MyPlayerHUD->MainMenuClass)
	{
		UMainMenu* BW = Cast<UMainMenu>(CreateWidget(PC,MyPlayerHUD->MainMenuClass));
		if(BW)
		{
			//Pause the game until menu is closed
			UGameplayStatics::SetGamePaused(this,true);
			BW->AddToViewport();
		}
	}
}

void APlayerCharacter::MoveForward(float MovementSpeed)
{
    if(CanMoveForward)
	{AddMovementInput(GetActorForwardVector(), MovementSpeed);}
}

void APlayerCharacter::MoveBackward(float MovementSpeed)
{
	if(CanMoveBackward)
	{AddMovementInput(-GetActorForwardVector(), MovementSpeed);}
}

void APlayerCharacter::MoveRight(float MovementSpeed)
{
	if(CanMoveRight)
	{AddMovementInput(GetActorRightVector(), MovementSpeed);}
}

void APlayerCharacter::MoveLeft(float MovementSpeed)
{
	if(CanMoveLeft)
	{AddMovementInput(-GetActorRightVector(), MovementSpeed);}
}

void APlayerCharacter::StartFire()
{
	if (ControlledWeapon != nullptr)
	{
		if (ControlledWeapon->GetWeaponFireMode() && ControlledWeapon->GetRemainBulletInMag() > 0)//Check the weapon if automatic fire mode on
		{
			IsFiring = true;
			GetWorldTimerManager().SetTimer(TimerHandle_WeaponAutomaticFire, this, &APlayerCharacter::Fire, ControlledWeapon->GetBulletInSec(), true, 0.0f);
		}
		else
		{
			if (GetWorld()->TimeSeconds - LastWeaponFire > ControlledWeapon->GetBulletInSec() && ControlledWeapon->GetRemainBulletInMag() > 0)//Check if gun is firing more than it's fire rate
			{
				IsFiring = true;
				LastWeaponFire = GetWorld()->TimeSeconds;
				Fire();
			}
		}
	}
}

void APlayerCharacter::StopFire()
{
	IsFiring = false;
	RecoilCurve=0.1f;
	GetWorldTimerManager().ClearTimer(TimerHandle_WeaponAutomaticFire);
}

void APlayerCharacter::Fire()
{
	//Check if player is reloading && Stop fire if mag is empty
	if (ControlledWeapon && !bIsReloadActive && ControlledWeapon->GetRemainBulletInMag() > 0 )
	{
		RecoilCurve = FMath::Clamp(RecoilCurve*2,0.0f,1.6f);
		ControlledWeapon->SetRemainingBulletInMag(ControlledWeapon->GetRemainBulletInMag() - 1);
		ControlledWeapon->Fire_VFX();
		ControlledWeapon->PlayFireSound();
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 0.0f, "Noise");
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(FireCamShake,0.5f);
		if (PlayerWeapon_LineTrace())
		{
			TWeakObjectPtr<AEnemyCharacter> AI = Cast<AEnemyCharacter>(WeaponLineTrace_HitResult.GetActor());
			/*Check linetrace result has any enemy character
			 * Also be sure line trace result is not pointing our character
			 * to prevent unwanted self damage
			 */
			if (AI.IsValid() && WeaponLineTrace_HitResult.GetActor()!=this)
			{
				ControlledWeapon->Impact_VFX(WeaponLineTrace_HitResult);//TODO add more surface
				float EffectiveDamage;
				EffectiveDamage = ControlledWeapon->GetWeaponDamage();
				PlayHitMarker();
				TWeakObjectPtr<AActor>(EnemyChar) = WeaponLineTrace_HitResult.Actor;
				/*if (WeaponLineTrace_HitResult.BoneName == "head")*///TODO add damage multiplier
				EnemyChar->TakeDamage(EffectiveDamage, DmgEvent, GetController(), this);
			}
		}
		if(!ControlledWeapon->GetWeaponFireMode())
		{
			StopFire();
		}
	}
	else
	{
		StopFire();
	}
}

//Set current character speed to specified running speed
void APlayerCharacter::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

//Set character speed to specified walking speed
void APlayerCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

//Reload current weapon
void APlayerCharacter::Reload()
{
	int32 AmmoIndex = PlayerInventory->ItemFinder("Ammunition");
	if(AmmoIndex==999)
	{
		return;
	}
	if (PlayerInventory->Inventory[AmmoIndex].ItemQuantity > 0 && (ControlledWeapon->GetRemainBulletInMag() != ControlledWeapon->GetMaximumMagazine()))
	{
		PlayReloadAnim();
	}
}

void APlayerCharacter::BeginCrouch()//TODO fix crouch
{
	Crouch();
}

void APlayerCharacter::EndCrouch()//TODO fix crouch
{
	UnCrouch();
}

//Switch weapon fire mode between automatic and single
void APlayerCharacter::ChangeWeaponFireMode()
{
	if (ControlledWeapon)
	{
		if (ControlledWeapon->GetWeaponFireModeInfo())
		{
			ControlledWeapon->SetWeaponFireMode(!ControlledWeapon->GetWeaponFireMode());
		}
	}
}

void APlayerCharacter::PickUp()//TODO Fix Interact function for all interactable object by using Interface
{
	if (OverlappedGun && OverlappedGun->IsA(APlayerWeapon::StaticClass()))
	{
		if (ControlledWeapon == nullptr && OverlappedGun != nullptr)
		{
			ControlledWeapon = OverlappedGun;
			PrepareControlledWeapon(ControlledWeapon);
		}
		else if (ControlledWeapon != nullptr && OverlappedGun != ControlledWeapon && OverlappedGun != nullptr)
		{
			APlayerWeapon* Swap = ControlledWeapon;
			OverlappedGun->DisableComponentsSimulatePhysics();
			ControlledWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ControlledWeapon = OverlappedGun;
			PrepareControlledWeapon(ControlledWeapon);
			Swap->SetActorEnableCollision(true);
			Swap->GetWeaponMesh()->SetSimulatePhysics(true);
			Swap->SetOwner(nullptr);
		}
	}
	else if (OverlappedItem && OverlappedItem->IsA(APickUpItem::StaticClass()) && PlayerInventory)
	{
		FString ItemName = OverlappedItem->GetClass()->GetDisplayNameText().ToString();
		PlayerInventory->AddItem(ItemName, OverlappedItem->GetPickUpItemTexture(),1);
		OverlappedItem->Destroy();
	}
	else
	{
		Interact();
	}
}

void APlayerCharacter::ReloadMagazine()
{
	//Find ammunition index from player inventory
	int32 AmmoIndex = PlayerInventory->ItemFinder("Ammunition");
	
	if(AmmoIndex==999)
	{
		return;
	}
	/*Check if player has ammo in inventory
	 *and player weapons mag condition
	 */
	if (PlayerInventory->Inventory[AmmoIndex].ItemQuantity > 0 && (ControlledWeapon->GetRemainBulletInMag() != ControlledWeapon->GetMaximumMagazine()))
	{
		//Getting remaining bullet in mag, so remaining bullets in mag wont be destroyed
		uint8 LeftBulletCount = ControlledWeapon->GetRemainBulletInMag();
		if ((ControlledWeapon->GetMaximumMagazine() - LeftBulletCount) >= PlayerInventory->Inventory[AmmoIndex].ItemQuantity)
		{
			ControlledWeapon->SetRemainingBulletInMag(PlayerInventory->Inventory[AmmoIndex].ItemQuantity+LeftBulletCount);
			PlayerInventory->RemoveItem(AmmoIndex,PlayerInventory->Inventory[AmmoIndex].ItemQuantity);
			bIsReloadActive = false;
		}
		else
		{
			PlayerInventory->RemoveItem(AmmoIndex,(ControlledWeapon->GetMaximumMagazine() - LeftBulletCount));
			ControlledWeapon->SetRemainingBulletInMag(ControlledWeapon->GetMaximumMagazine());
			bIsReloadActive = false;
		}
	}
}

//Check fall damage
void APlayerCharacter::ApplyFallDamage()
{
	if(GetCharacterMovement()->IsFalling() && !FallDamageFlag)
	{
		FallDamageFlag = true;
		LocationBeforeFall = GetActorLocation();
	}
	/*Player character needs to take fall damage
	* but fall damage also needs a threshold so it
	* wont be applied when jumping around
	*/
	if(!GetCharacterMovement()->IsFalling() && FallDamageFlag)
	{
		LocationAfterFall = GetActorLocation();
		float FallDistance = FMath::Abs(LocationBeforeFall.Z - LocationAfterFall.Z);
		if(FallDistance > 100)
		{
			int FallDamage = FMath::RoundToInt(FMath::Exp(FMath::LogX(20,FallDistance)));
			FDamageEvent DamEv;
			if(FallDamage>10)
			TakeDamage(FallDamage,DamEv,GetInstigatorController(),this);
		}
		FallDamageFlag = false;
	}
}

bool APlayerCharacter::PlayerWeapon_LineTrace()
{
	if (CameraComp && ControlledWeapon)
	{
		FVector LineTraceStartLocation = ControlledWeapon->GetMuzzleSocketLocation();
		FRotator LineTraceStartRotation = ControlledWeapon->GetADSRotation();
		FVector ShotDirection = LineTraceStartRotation.Vector();
		float HalfRad = FMath::DegreesToRadians(ControlledWeapon->GetWeaponSpreadRatio());
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);
		FVector LineTraceEndVector = LineTraceStartLocation + (ShotDirection * ControlledWeapon->GetLineTraceDistance());
		bool bIsLineTraceSucceed = GetWorld()->LineTraceSingleByChannel(WeaponLineTrace_HitResult,
			LineTraceStartLocation,
			LineTraceEndVector, ECollisionChannel::ECC_Pawn, QParams
		);
		return bIsLineTraceSucceed;
	}
	return false;
}

void APlayerCharacter::UseHealthPotion()//TODO Move this to InteractableItem
{
	int32 ItemIndex = PlayerInventory->ItemFinder("Health Potion");
	if (ItemIndex != 999 && PlayerInventory->GetInventoryPlayerCharacter()[ItemIndex].ItemQuantity > 0 && PlayerHealth->GetCurrentHP() != 100)
	{
		PlayerHealth->HealActor(20.0f);
		PlayerInventory->RemoveItem(ItemIndex, 1);
	}
}

//Fire animation
void APlayerCharacter::PlayFireAnim()
{
	if(bIsAiming)
	{
		PlayAnimMontage(AimFire_Montage);
	}
	else
	{
		PlayAnimMontage(HipFire_Montage);
	}
}

//Reload animation
void APlayerCharacter::PlayReloadAnim()
{
	if(bIsAiming)
	{
		PlayAnimMontage(AimReload_Montage);
	}
	else
	{
		PlayAnimMontage(HipReload_Montage);
	}
}

void APlayerCharacter::PrepareControlledWeapon(APlayerWeapon* TargetWeapon)
{
	TargetWeapon->GetWeaponMesh()->SetSimulatePhysics(false);
	TargetWeapon->SetActorEnableCollision(false);
	TargetWeapon->SetOwner(this);
	TargetWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	ADSCameraComp->AttachToComponent(TargetWeapon->GetWeaponMesh(),FAttachmentTransformRules::KeepRelativeTransform,TargetWeapon->GetADSSocketName());
}

void APlayerCharacter::Jump()
{
	if(CanJumpp)
	{
		Super::Jump();
		IsJumped=true;
	}
}

void APlayerCharacter::DisableMoving()
{
	CanMoveForward=false;
	CanMoveBackward=false;
	CanMoveLeft=false;
	CanMoveRight=false;
	CanJumpp=false;
}

void APlayerCharacter::EnableMoving()
{
	CanMoveForward=true;
	CanMoveBackward=true;
	CanMoveLeft=true;
	CanMoveRight=true;
	CanJumpp=true;
}

void APlayerCharacter::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerWeapon::StaticClass()))
	{
		OverlappedGun = Cast<APlayerWeapon>(OtherActor);
	}
	if (OtherActor->IsA(APickUpItem::StaticClass()))
	{
		OverlappedItem = Cast<APickUpItem>(OtherActor);
	}
}

void APlayerCharacter::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	OverlappedGun = nullptr;
	OverlappedItem = nullptr;
}

void APlayerCharacter::AddItemToPlayerInventory(FString ItemName, UTexture2D* ItemTexture,int32 ItemQuantity)
{
	PlayerInventory->AddItem(ItemName,ItemTexture,ItemQuantity);
}

void APlayerCharacter::SetPlayerInputs()
{
	if(PlayerInputComp)
	{
		PlayerInputComp->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
		PlayerInputComp->BindAxis("MoveBackward", this, &APlayerCharacter::MoveBackward);
		PlayerInputComp->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
		PlayerInputComp->BindAxis("MoveLeft", this, &APlayerCharacter::MoveLeft);
		PlayerInputComp->BindAxis("MouseHorizontal", this, &APlayerCharacter::AddControllerYawInput);
		PlayerInputComp->BindAxis("MouseVertical", this, &APlayerCharacter::AddControllerPitchInput);
		PlayerInputComp->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
		PlayerInputComp->BindAction("Run",EInputEvent::IE_Pressed,this,&APlayerCharacter::StartRun);
		PlayerInputComp->BindAction("Run",EInputEvent::IE_Released,this,&APlayerCharacter::StopRun);
		PlayerInputComp->BindAction("Crouch", EInputEvent::IE_Pressed, this, &APlayerCharacter::BeginCrouch);
		PlayerInputComp->BindAction("Crouch", EInputEvent::IE_Released, this, &APlayerCharacter::EndCrouch);
		PlayerInputComp->BindAction("Aiming", EInputEvent::IE_Pressed, this, &APlayerCharacter::Aiming);
		PlayerInputComp->BindAction("Fire", EInputEvent::IE_Pressed, this, &APlayerCharacter::StartFire);
		PlayerInputComp->BindAction("Fire", EInputEvent::IE_Released, this, &APlayerCharacter::StopFire);
		PlayerInputComp->BindAction("Reload", EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);
		PlayerInputComp->BindAction("WeaponFireModeChange", EInputEvent::IE_Pressed, this, &APlayerCharacter::ChangeWeaponFireMode);
		PlayerInputComp->BindAction("PickUp", EInputEvent::IE_Pressed, this, &APlayerCharacter::PickUp);
		PlayerInputComp->BindAction("Inventory", EInputEvent::IE_Pressed, this, &APlayerCharacter::ShowInventory);
		PlayerInputComp->BindAction("UseHealthPotion", EInputEvent::IE_Pressed, this, &APlayerCharacter::UseHealthPotion);
		PlayerInputComp->BindAction("OpenMainMenu", EInputEvent::IE_Pressed, this, &APlayerCharacter::MainMenu);
	}
}

void APlayerCharacter::DecreaseCoin(int32 DecreaseAmount)
{
	PlayerCoin = FMath::Clamp(PlayerCoin-DecreaseAmount,0,9999);
}

int32 APlayerCharacter::GetPlayerCoin()
{
	return PlayerCoin;
}

void APlayerCharacter::GetCoinByKill(AActor* VictimActor, AActor* KillerActor, AController* KillerController)
{
	TempCharacter = Cast<APlayerCharacter>(KillerActor);
	if(TempCharacter)
	{
		TempCharacter->DecreaseCoin(-100);
	}
}

