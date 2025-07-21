// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu.h"

void UPauseMenu::NativeOnActivated()
{
	Super::NativeOnActivated();
	OpenedTime = GetWorld()->GetRealTimeSeconds();
}

FReply UPauseMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const float Now = GetWorld()->GetRealTimeSeconds();
	const float TimeSinceOpen = Now - OpenedTime;

	if (TimeSinceOpen < CloseCooldownTime || InKeyEvent.IsRepeat())
		return FReply::Handled();
	
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right)
	{
		NativeOnHandleBackAction(); // close the pause menu
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
