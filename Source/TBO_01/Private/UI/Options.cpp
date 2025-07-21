// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options.h"
#include "Journal/ActionWidget.h"
#include "UI/OptionsTab.h"

void UOptions::NativeOnActivated()
{
	Super::NativeOnActivated();
	OpenedTime = GetWorld()->GetRealTimeSeconds();
}

FReply UOptions::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const float Now = GetWorld()->GetRealTimeSeconds();
	const float TimeSinceOpen = Now - OpenedTime;
	
	if (TimeSinceOpen < CloseCooldownTime || InKeyEvent.IsRepeat())
		return FReply::Handled();
	
	const FKey Key = InKeyEvent.GetKey();
	if (Key == W_OptionsTabList -> LeftTab -> KeyboardKey || Key == W_OptionsTabList -> LeftTab -> GamepadKey)
	{
		UE_LOG(LogTemp, Warning, TEXT("Options Tab Left Clicked"));
		W_OptionsTabList -> LeftTab -> PlayActionAnimation();
		return FReply::Unhandled();
	}
	if (Key == W_OptionsTabList -> RightTab -> KeyboardKey || Key == W_OptionsTabList -> RightTab -> GamepadKey)
	{
		UE_LOG(LogTemp, Warning, TEXT("Options Tab Right Clicked"));
		W_OptionsTabList -> RightTab -> PlayActionAnimation();
		return FReply::Unhandled();
	}
	if (Key == EKeys::X || Key == EKeys::Gamepad_FaceButton_Right)
	{
		NativeOnHandleBackAction(); // close the pause menu
		return FReply::Handled();
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}
