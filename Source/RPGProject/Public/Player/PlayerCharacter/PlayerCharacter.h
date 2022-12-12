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

	void MoveForward(float MovementSpeed);

	void MoveBackward(float MovementSpeed);

	void MoveRight(float MovementSpeed);

	void MoveLeft(float MovementSpeed);

	void Fire();

	void StartRun();

	void StopRun();

	void Reload();

	void BeginCrouch();

	void EndCrouch();

	void ChangeWeaponFireMode();

	void PickUp();
	
	bool PlayerWeapon_LineTrace();

	void UseHealthPotion();

	void PlayFireAnim();

	void PlayReloadAnim();

	void PrepareControlledWeapon(APlayerWeapon* TargetWeapon);

	FInputActionBinding myActionBinding;

	virtual void Jump() override;

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

	float LastWeaponFire;

	FVector LocationBeforeFall,LocationAfterFall;

	bool FallDamageFlag;

	FString LastItemName;

	FHitResult WeaponLineTrace_HitResult;

	FTimerHandle TimerHandle_WeaponAutomaticFire;

	FTimerHandle TimerHandle_WeaponSingleFire;

	FDamageEvent DmgEvent;

	float RecoilCurve;

	AWidgetPlayerController* PC;
	
	UMyGameInstance* GI;

	APlayerHUD* MyPlayerHUD;
	
	APlayerController* PlayerCont;

	FCollisionQueryParams QParams;

	int32* AmmoOnPlayer = nullptr;

	UPROPERTY(BlueprintReadWrite)
	APlayerWeapon* ControlledWeapon = nullptr;

	FName WeaponSocketName;

	APlayerWeapon* OverlappedGun;

	APickUpItem* OverlappedItem;

	APlayerCharacter* TempCharacter;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Montage")
		UAnimMontage* AimFire_Montage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Montage")
		UAnimMontage* HipFire_Montage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Montage")
		UAnimMontage* AimReload_Montage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Montage")
		UAnimMontage* HipReload_Montage;
	
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

	UFUNCTION(BlueprintCallable)
		void StartFire();

	UFUNCTION(BlueprintCallable)
		void StopFire();
	
	UFUNCTION(BlueprintImplementableEvent)
		void ShowInventory();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayHitMarker();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Player")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Player")
		UCameraComponent* CameraComp;
		
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Player")
		UCameraComponent* ADSCameraComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<APlayerWeapon> WeaponClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Inventory")
		UPlayerInventory* PlayerInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		UPlayerCharacter_HealthComponent* PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
		UPlayerQuests* PlayerQuests;
	
	UFUNCTION(BlueprintImplementableEvent)
		void Aiming();

	UFUNCTION(BlueprintCallable)
		int32 GetCurrentAmmo();

	UFUNCTION(BlueprintCallable)
		void MainMenu();

	UFUNCTION(BlueprintImplementableEvent)
		void Interact();

	UFUNCTION()
		void GetCoinByKill(AActor* VictimActor, AActor* KillerActor, AController* KillerController);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DecreaseCoin(int32 DecreaseAmount);

	int32 GetPlayerCoin();
	
	void ReloadMagazine();

	void ApplyFallDamage();

	void AddItemToPlayerInventory(FString ItemName, UTexture2D* ItemTexture,int32 ItemQuantity);

	void SetPlayerInputs();

	UPlayerInventory* GetInventoryComponent();

	UPlayerCharacter_HealthComponent* GetHealthComponent();

	UPROPERTY()
	FOnWeaponEquipped OnWeaponEquipped;

	UPROPERTY()
	FOnCharacterFired OnCharacterFired;

	UPROPERTY(EditAnywhere,Category="Camera")
	TSubclassOf<UMatineeCameraShake> FireCamShake;

	UFUNCTION(BlueprintCallable)
		uint8 GetWeaponAmmo();
	
	UFUNCTION(BlueprintCallable)
		uint8 GetMaxMagSize();

	UFUNCTION(BlueprintCallable)
		void DisableMoving();

	UFUNCTION(BlueprintCallable)
		void EnableMoving();
	
	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
		void RefreshInventory();

	UFUNCTION(BlueprintImplementableEvent)
		void RefreshActiveQuest();
};
