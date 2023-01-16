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
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericPlatform/GenericApplication.h"
#include "Perception/AISense_Hearing.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Widget/Widget/MainMenu.h"
#include "MyGameModeBase.h"
#include "Widget/WidgetBase/MainMenu_BaseWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	this->OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	this->OnActorEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

	DefaultCharacterLocationVector = FVector(0.0f,0.0f,-90.0f);
	DefaultCharacterRotation = FRotator(0.0f,0.0f,-90.f);
	
	GetMesh()->SetRelativeLocation(DefaultCharacterLocationVector);
	GetMesh()->SetRelativeRotation(DefaultCharacterRotation);
	
	WeaponSocketName = "WeaponSocket";
	EyeSocketName="EyeSocket";

	AmmoItemID = "0101";

	QParams = FCollisionQueryParams::DefaultQueryParam;
	QParams.AddIgnoredActor(this);
	QParams.bReturnPhysicalMaterial = true;
	QParams.bTraceComplex = true;

	DefaultSpringArmVector = FVector(0.0f,0.0f,70.0f);
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(DefaultSpringArmVector);
	SpringArmComp->TargetArmLength = 0.0f;

	//Third person camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	DefaultCameraVector = FVector(-170.0f,46.0f,-15.0f);
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
	CameraComp->SetRelativeLocation(DefaultCameraVector);

	//First person camera
	ADSCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ADSCameraComp"));
	ADSCameraComp->SetRelativeScale3D(FVector(0.1f));
	ADSCameraComp->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepWorldTransform,EyeSocketName);
	ADSCameraComp->SetWorldRotation(GetMesh()->GetSocketRotation(EyeSocketName));
	
	//Player components
	PlayerHealth = CreateDefaultSubobject<UPlayerCharacter_HealthComponent>(TEXT("HealthComponent"));
	PlayerInventory = CreateDefaultSubobject<UPlayerInventory>(TEXT("InventoryComponent"));
	PlayerQuests = CreateDefaultSubobject<UPlayerQuests>(TEXT("QuestComponent"));
	
	PlayerHealth->OnActorKilled.AddDynamic(this,&APlayerCharacter::APlayerCharacter::GetCoinByKill);

	//Camera transition duration
	AdsDuration = 0.5f;
	PlayerCoin = 500;
	RecoilCurve = 0.1f;
	WalkSpeed = 300.0f;
	RunSpeed = 600.0f;
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<AWidgetPlayerController>(UGameplayStatics::GetPlayerController(this,0));
	GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	MyPlayerHUD = Cast<APlayerHUD>(PC->GetHUD());
	
	ADSCameraComp->SetActive(false);

	GI->LoadGame();
	
	LastWeaponFire = GetWorld()->TimeSeconds;

	PlayerCont=UGameplayStatics::GetPlayerController(GetWorld(),0);
	EnableMoving();
	
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

void APlayerCharacter::Aim()
{
	if(ControlledWeapon)
	{
		if(bIsAiming)
		{
			StopAiming();
		}
		else
		{
			StartAiming();
		}
	}
}

void APlayerCharacter::StartAiming()
{
	AdsTimeCounter = 0.0f;
	GetWorldTimerManager().SetTimer(TimerHandle_AdsCam,this,&APlayerCharacter::SetAdsCam,0.01f,true,0.1f);
	bIsAiming=true;
}

void APlayerCharacter::StopAiming()
{
	AdsTimeCounter = 0.0f;
	CameraComp->SetWorldLocationAndRotation(ADSCameraComp->GetComponentLocation(),ADSCameraComp->GetComponentRotation());
	ADSCameraComp->SetActive(false);
	CameraComp->SetActive(true);
	GetWorldTimerManager().SetTimer(TimerHandle_AdsCam,this,&APlayerCharacter::ReverseAdsCam,0.01f,true,0.1f);
	bIsAiming=false;
}

void APlayerCharacter::SetAdsCam()
{
	if(AdsTimeCounter<AdsDuration)
	{
		float LerpAlpha = AdsTimeCounter/AdsDuration;
		CameraComp->SetWorldLocation(FMath::Lerp(CameraComp->GetComponentLocation(),ADSCameraComp->GetComponentLocation(),LerpAlpha));
		CameraComp->SetWorldRotation(FMath::Lerp(CameraComp->GetComponentRotation(),ADSCameraComp->GetComponentRotation(),LerpAlpha));
		AdsTimeCounter += 0.01f;
	}
	else
	{
		CameraComp->SetActive(false);
		ADSCameraComp->SetActive(true);
		GetWorldTimerManager().ClearTimer(TimerHandle_AdsCam);
	}
}

void APlayerCharacter::ReverseAdsCam()
{
	if(AdsTimeCounter<AdsDuration)
	{
		float LerpAlpha = AdsTimeCounter/AdsDuration;
		CameraComp->SetRelativeLocation(FMath::Lerp(CameraComp->GetRelativeLocation(),DefaultCameraVector,LerpAlpha));
		CameraComp->SetRelativeRotation(FMath::Lerp(CameraComp->GetRelativeRotation(),FRotator(0.0f),LerpAlpha));
		AdsTimeCounter += 0.01f;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_AdsCam);
	}
}

//Get current ammunition on player
int32 APlayerCharacter::GetCurrentAmmo()
{
	bool bIsItemFound;
	int32 AmmoIndex = PlayerInventory->ItemIndexFinder(AmmoItemID,bIsItemFound);
	if(bIsItemFound)
		return PlayerInventory->GetInventoryItems()[AmmoIndex].ItemQuantity;
	
	return 0;
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
    {
	    AddMovementInput(GetActorForwardVector(), MovementSpeed);
    }
}

void APlayerCharacter::MoveBackward(float MovementSpeed)
{
	if(CanMoveBackward)
	{
		AddMovementInput(-GetActorForwardVector(), MovementSpeed);
	}
}

void APlayerCharacter::MoveRight(float MovementSpeed)
{
	if(CanMoveRight)
	{
		AddMovementInput(GetActorRightVector(), MovementSpeed);
	}
}

void APlayerCharacter::MoveLeft(float MovementSpeed)
{
	if(CanMoveLeft)
	{
		AddMovementInput(-GetActorRightVector(), MovementSpeed);
	}
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
	if (ControlledWeapon && (PlayerState!=Reloading) && ControlledWeapon->GetRemainBulletInMag() > 0 )
	{
		RecoilCurve = FMath::Clamp(RecoilCurve*2,0.0f,1.6f);
		ControlledWeapon->SetRemainingBulletInMag(ControlledWeapon->GetRemainBulletInMag() - 1);
		OnCharacterFired.Broadcast();
		ControlledWeapon->Fire_VFX();
		ControlledWeapon->PlayFireSound();
		PlayFireAnim();
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
	bool bIsItemFound;
	int32 AmmoIndex = PlayerInventory->ItemIndexFinder(AmmoItemID,bIsItemFound);
	if (bIsItemFound && PlayerInventory->GetInventoryItems()[AmmoIndex].ItemQuantity > 0 && (ControlledWeapon->GetRemainBulletInMag() != ControlledWeapon->GetMaximumMagazine()))
	{
		PlayerState = EPlayerState::Reloading;
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

void APlayerCharacter::PickUp()
{
	if(!OverlappedItem)
		return;
	if(IInterface_InteractableItem* InteractableItemObject = Cast<IInterface_InteractableItem>(OverlappedItem))
	{
		InteractableItemObject->Interact(this);
		MyPlayerHUD->SetInteract(ESlateVisibility::Hidden);
	}
}

void APlayerCharacter::ReloadMagazine()
{
	bool bIsItemFound;
	//Find ammunition index from player inventory
	int32 AmmoIndex = PlayerInventory->ItemIndexFinder(AmmoItemID,bIsItemFound);
	
	/*Check if player has ammo in inventory
	 *and player weapons mag condition
	 */
	if (bIsItemFound && PlayerInventory->GetInventoryItems()[AmmoIndex].ItemQuantity > 0 && (ControlledWeapon->GetRemainBulletInMag() != ControlledWeapon->GetMaximumMagazine()))
	{
		//Getting remaining bullet in mag, so remaining bullets in mag wont be destroyed
		uint8 LeftBulletCount = ControlledWeapon->GetRemainBulletInMag();
		if ((ControlledWeapon->GetMaximumMagazine() - LeftBulletCount) >= PlayerInventory->GetInventoryItems()[AmmoIndex].ItemQuantity)
		{
			ControlledWeapon->SetRemainingBulletInMag(PlayerInventory->GetInventoryItems()[AmmoIndex].ItemQuantity+LeftBulletCount);
			PlayerInventory->RemoveItem(AmmoItemID,PlayerInventory->GetInventoryItems()[AmmoIndex].ItemQuantity);
			bIsReloadActive = false;
		}
		else
		{
			ControlledWeapon->SetRemainingBulletInMag(ControlledWeapon->GetMaximumMagazine());
			PlayerInventory->RemoveItem(AmmoItemID,(ControlledWeapon->GetMaximumMagazine() - LeftBulletCount));
			bIsReloadActive = false;
		}
	}
	PlayerState = EPlayerState::Idle;
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

UPlayerInventory* APlayerCharacter::GetInventoryComponent()
{
	return PlayerInventory;
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

//Fire animation
void APlayerCharacter::PlayFireAnim()
{
	if(bIsAiming)
	{
		PlayAnimMontage(AimFire_Montage,0.8f);
	}
	else
	{
		PlayAnimMontage(HipFire_Montage,0.75f);
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
	if(ControlledWeapon==nullptr)
	{
		TargetWeapon->GetWeaponMesh()->SetSimulatePhysics(false);
		TargetWeapon->SetActorEnableCollision(false);
		// TargetWeapon->SetOwner(this);
		TargetWeapon->SetWeaponOwner(this);
		TargetWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		ControlledWeapon = TargetWeapon;
		OnWeaponEquipped.Broadcast(true,TargetWeapon);
	}
	else
	{
		ControlledWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ControlledWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
		ControlledWeapon->SetActorEnableCollision(true);
		ControlledWeapon->SetWeaponOwner(nullptr);
		ControlledWeapon->SetActorLocationAndRotation(TargetWeapon->GetActorLocation(),TargetWeapon->GetActorRotation());
		ControlledWeapon=nullptr;
		PrepareControlledWeapon(TargetWeapon);
	}
}

void APlayerCharacter::Jump()
{
	if(CanJumpp)
	{
		Super::Jump();
		IsJumped=true;
	}
}

UPlayerCharacter_HealthComponent* APlayerCharacter::GetHealthComponent()
{
	return PlayerHealth;
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
	OverlappedItem = OtherActor;
	if(Cast<IInterface_InteractableItem>(OtherActor))
	{
		MyPlayerHUD->SetInteract(ESlateVisibility::Visible,Cast<IInterface_InteractableItem>(OtherActor)->GetItemName());
	}
}

void APlayerCharacter::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	OverlappedGun = nullptr;
	OverlappedItem = nullptr;
	MyPlayerHUD->SetInteract(ESlateVisibility::Hidden);
}

//Add item to player inventory
//Use this method if quantity of item is set in item data asset
void APlayerCharacter::AddItemToPlayerInventory(FName ItemID)
{
	AMyGameModeBase* MyGameModeBase;
	MyGameModeBase = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	if(MyGameModeBase)
	{
		bool bIsItemFound = false;
		FItem TempItem;
		TempItem = MyGameModeBase->FindItem(ItemID,bIsItemFound);
		if(bIsItemFound && PlayerInventory)
			PlayerInventory->AddItem(TempItem);
	}
}

//Add item to player inventory
//Use this method when quantity is different from default item data asset
void APlayerCharacter::AddItemToPlayerInventory(FName ItemID, int32 QuantityToAdd)
{
	AMyGameModeBase* MyGameModeBase;
	MyGameModeBase = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	if(MyGameModeBase)
	{
		bool bIsItemFound = false;
		FItem TempItem;
		TempItem = MyGameModeBase->FindItem(ItemID,bIsItemFound);
		if(bIsItemFound && PlayerInventory)
		{
			TempItem.ItemQuantity = QuantityToAdd;
			PlayerInventory->AddItem(TempItem);
		}
	}
}

//Drop inventory item
void APlayerCharacter::DropItemFromPlayerInventory(FName ItemID, int32 QuantityToDrop)
{
	AMyGameModeBase* MyGameModeBase;
	MyGameModeBase = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	if(MyGameModeBase)
	{
		bool bIsItemFound = false;
		FItem TempItem;
		TempItem = MyGameModeBase->FindItem(ItemID,bIsItemFound);
		if(bIsItemFound && PlayerInventory)
		{
			PlayerInventory->RemoveItem(ItemID,QuantityToDrop);
			FVector ItemLoc = GetMesh()->GetBoneLocation("ik_foot_root");
			APickUpItem* TempPickUpItem = Cast<APickUpItem>(GetWorld()->SpawnActor(TempItem.ItemClass,&ItemLoc));
			TempPickUpItem->SetQuantity(QuantityToDrop);
		}
	}
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
		PlayerInputComp->BindAction("Aiming", EInputEvent::IE_Pressed, this, &APlayerCharacter::Aim);
		PlayerInputComp->BindAction("Fire", EInputEvent::IE_Pressed, this, &APlayerCharacter::StartFire);
		PlayerInputComp->BindAction("Fire", EInputEvent::IE_Released, this, &APlayerCharacter::StopFire);
		PlayerInputComp->BindAction("Reload", EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);
		PlayerInputComp->BindAction("WeaponFireModeChange", EInputEvent::IE_Pressed, this, &APlayerCharacter::ChangeWeaponFireMode);
		PlayerInputComp->BindAction("PickUp", EInputEvent::IE_Pressed, this, &APlayerCharacter::PickUp);
		PlayerInputComp->BindAction("Inventory", EInputEvent::IE_Pressed, this, &APlayerCharacter::ShowInventory);
		PlayerInputComp->BindAction("OpenMainMenu", EInputEvent::IE_Pressed, this, &APlayerCharacter::MainMenu);
		PlayerInputComp->BindAction("SaveGame", EInputEvent::IE_Pressed, this, &APlayerCharacter::SaveGame);
	}
}

UClass* APlayerCharacter::GetWeaponClass()
{
	if(ControlledWeapon)
	{
		return ControlledWeapon->GetClass();
	}

	return nullptr;
}

void APlayerCharacter::DecreaseCoin(int32 DecreaseAmount)
{
	PlayerCoin = FMath::Clamp(PlayerCoin-DecreaseAmount,0,9999);
}

void APlayerCharacter::SaveGame()
{
	GI->SaveGame();
}

void APlayerCharacter::GetCoinByKill(AActor* VictimActor, AActor* KillerActor, AController* KillerController)
{
	TempCharacter = Cast<APlayerCharacter>(KillerActor);
	if(TempCharacter)
	{
		TempCharacter->DecreaseCoin(-100);
	}
}

