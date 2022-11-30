// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter/PlayerCharacterFire_CameraShake.h"

UPlayerCharacterFire_CameraShake::UPlayerCharacterFire_CameraShake()
{
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = 1.0f;
	RotOscillation.Pitch.Frequency = 3.0f;

	RotOscillation.Yaw.Amplitude = 1.0f;
	RotOscillation.Yaw.Frequency = 3.0f;

	LocOscillation.Y.Amplitude = 1.0f;
	LocOscillation.Y.Frequency = 1.0f;
	LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
	
	LocOscillation.Z.Amplitude = 1.0f;
	LocOscillation.Z.Frequency=1.0f;
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

}
