// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class APickUpItem;
class UPlayerInventory;
class UPlayerCharacter_HealthComponent;
class APlayerWeapon;
class UCameraComponent;
class USpringArmComponent;
class UPlayerQuests;
class UMyGameInstance;
class APlayerHUD;
class AWidgetPlayerController;
struct FItem;

UENUM()
enum EPlayerState
{
	Idle,
	Aiming,
	Reloading,
	Jumping
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEquipped, bool, bIsEquipped, APlayerWeapon*, PlayerWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterFired);

UCLASS(Blueprintable)
class RPGPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	// Sets default values for this character's properties
	APlayerCharacter();

	UInputComponent* PlayerInputComp;
	
protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Movement
	void MoveForward(float MovementSpeed);

	void MoveBackward(float MovementSpeed);

	void MoveRight(float MovementSpeed);

	void MoveLeft(float MovementSpeed);

	virtual void Jump() override;

	void StartRun();

	void StopRun();

	void Reload();

	void BeginCrouch();

	void EndCrouch();

	void ChangeWeaponFireMode();

	void PickUp();

	void PlayFireAnim();

	void PlayReloadAnim();

	EPlayerState PlayerState;

	UPROPERTY(EditDefaultsOnly,Category="Movement")
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly,Category="Movement")
	float RunSpeed;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	bool CanMoveForward;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	bool CanMoveBackward;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	bool CanMoveRight;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	bool CanMoveLeft;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	bool CanJumpp;


	//Weapon&Fire

	void Fire();
	
	bool PlayerWeapon_LineTrace();

	void StartAiming();
	
	void StopAiming();

	void SetAdsCam();

	void ReverseAdsCam();

	float RecoilCurve;
	
	float LastWeaponFire;

	float AdsDuration;

	float AdsTimeCounter;

	FName AmmoItemID;

	FHitResult WeaponLineTrace_HitResult;

	FTimerHandle TimerHandle_AdsCam;

	FTimerHandle TimerHandle_WeaponAutomaticFire;

	UPROPERTY(BlueprintReadWrite)
	APlayerWeapon* ControlledWeapon = nullptr;

	UFUNCTION(BlueprintCallable)
		void StartFire();

	UFUNCTION(BlueprintCallable)
		void StopFire();
	

	//Declarations

	AWidgetPlayerController* PC;
	
	UMyGameInstance* GI;

	APlayerHUD* MyPlayerHUD;
	
	APlayerController* PlayerCont;

	APlayerWeapon* OverlappedGun;

	AActor* OverlappedItem;

	APlayerCharacter* TempCharacter;
	
	//
	
	FVector DefaultCameraVector, DefaultSpringArmVector, DefaultCharacterLocationVector;

	FRotator DefaultCharacterRotation;

	FName EyeSocketName;

	FVector LocationBeforeFall,LocationAfterFall;

	bool FallDamageFlag;

	FDamageEvent DmgEvent;

	FCollisionQueryParams QParams;

	FName WeaponSocketName;

	//Anim Montage
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Montage")
		UAnimMontage* AimFire_Montage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Montage")
		UAnimMontage* HipFire_Montage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Montage")
		UAnimMontage* AimReload_Montage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Montage")
		UAnimMontage* HipReload_Montage;

	//Blueprint
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Coin")
		int32 PlayerCoin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsAiming;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsReloadActive;

	UPROPERTY(BlueprintReadWrite)
		bool IsJumped = false;
	
	UFUNCTION(BlueprintImplementableEvent)
		void ShowInventory();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayHitMarker();

	//Components
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Player")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Player")
		UCameraComponent* CameraComp;
		
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Player")
		UCameraComponent* ADSCameraComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Inventory")
		UPlayerInventory* PlayerInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		UPlayerCharacter_HealthComponent* PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
		UPlayerQuests* PlayerQuests;


	//
	
	UFUNCTION(BlueprintCallable)
		void Aim();

	UFUNCTION(BlueprintCallable)
		int32 GetCurrentAmmo();

	UFUNCTION(BlueprintCallable)
		void MainMenu();

	UFUNCTION(BlueprintImplementableEvent)
		void Interact();

	UFUNCTION(BlueprintCallable)
		void SaveGame();

	UFUNCTION()
		void GetCoinByKill(AActor* VictimActor, AActor* KillerActor, AController* KillerController);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DecreaseCoin(int32 DecreaseAmount);
	
	void ReloadMagazine();

	void ApplyFallDamage();

	void PrepareControlledWeapon(APlayerWeapon* TargetWeapon);

	void AddItemToPlayerInventory(FName ItemID);

	void AddItemToPlayerInventory(FName ItemID, int32 QuantityToAdd);

	UPROPERTY()
	FOnWeaponEquipped OnWeaponEquipped;

	UPROPERTY()
	FOnCharacterFired OnCharacterFired;

	UPROPERTY(EditAnywhere,Category="Camera")
	TSubclassOf<UMatineeCameraShake> FireCamShake;

	UFUNCTION(BlueprintCallable)
		void DisableMoving();

	UFUNCTION(BlueprintCallable)
		void EnableMoving();
	
	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	//Get&Set

	void SetPlayerInputs();

	int32 GetPlayerCoin();

	UFUNCTION(BlueprintCallable)
		uint8 GetWeaponAmmo();
	
	UFUNCTION(BlueprintCallable)
		uint8 GetMaxMagSize();

	UClass* GetWeaponClass();

	UPlayerInventory* GetInventoryComponent();

	UPlayerCharacter_HealthComponent* GetHealthComponent();
};

