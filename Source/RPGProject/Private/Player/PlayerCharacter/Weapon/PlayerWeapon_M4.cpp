// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/Weapon/PlayerWeapon_M4.h"

APlayerWeapon_M4::APlayerWeapon_M4()
{
	LineTraceDistance =4000.0f;
	WeaponDamage=15.0f;
	WeaponFireRate=800.0f;
	MagSize=30;
	WeaponSpreadRatio=2.0f;
	SetWeaponName = "M4";
	bCanBeAutomatic=true;
	PitchRecoil = 0.25f;
	YawRecoil = 0.1f;
}
