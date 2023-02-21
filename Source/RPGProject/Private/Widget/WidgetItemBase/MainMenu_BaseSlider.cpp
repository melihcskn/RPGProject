// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetItemBase/MainMenu_BaseSlider.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/Slider.h"

void UMainMenu_BaseSlider::NativeConstruct()
{
	Super::NativeConstruct();

	SliderValue=0.0f;
	
	VolumeSLider->SetMinValue(0.0f);
	VolumeSLider->SetMaxValue(100.0f);
	VolumeSLider->SetStepSize(1.0f);
}

void UMainMenu_BaseSlider::Apply()
{
	IMenuItemsInterface::Apply();

	VolumeSLider->OnValueChanged.Broadcast(SliderValue);
}

void UMainMenu_BaseSlider::SetSelected()
{
	IMenuItemsInterface::SetSelected();

	SelectionBorder->SetBrushColor(FLinearColor::Red);
}

void UMainMenu_BaseSlider::UnSelect()
{
	IMenuItemsInterface::UnSelect();

	SelectionBorder->SetBrushColor(FLinearColor::Black);
}

void UMainMenu_BaseSlider::SwitchLeft()
{
	IMenuSwitchableItemInterface::SwitchLeft();

	SliderValue = FMath::Clamp(SliderValue-(VolumeSLider->GetStepSize()),VolumeSLider->GetMinValue(),VolumeSLider->GetMaxValue());
	VolumeSLider->SetValue(SliderValue);
}

void UMainMenu_BaseSlider::SwitchRight()
{
	IMenuSwitchableItemInterface::SwitchRight();

	SliderValue = FMath::Clamp(SliderValue+(VolumeSLider->GetStepSize()),VolumeSLider->GetMinValue(),VolumeSLider->GetMaxValue());
	VolumeSLider->SetValue(SliderValue);
}
