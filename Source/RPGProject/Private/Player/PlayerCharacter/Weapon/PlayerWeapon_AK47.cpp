// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/Weapon/PlayerWeapon_AK47.h"

APlayerWeapon_AK47::APlayerWeapon_AK47()
{
	LineTraceDistance =5000.0f;
	WeaponDamage=20.0f;
	WeaponFireRate=600.0f;
	MagSize=40;
	WeaponSpreadRatio=3.0f;
	SetWeaponName = "AK47";
	bCanBeAutomatic=true;
	PitchRecoil = 0.35f;
	YawRecoil = 0.15f;
}
