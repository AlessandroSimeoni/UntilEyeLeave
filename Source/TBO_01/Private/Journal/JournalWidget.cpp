// Fill out your copyright notice in the Description page of Project Settings.


#include "Journal/JournalWidget.h"

#include "Journal/ActionWidget.h"
#include "Journal/JournalTab.h"

void UJournalWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	OpenedTime = GetWorld()->GetRealTimeSeconds();
}

FReply UJournalWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const float Now = GetWorld()->GetRealTimeSeconds();
	const float TimeSinceOpen = Now - OpenedTime;

	if (TimeSinceOpen < CloseCooldownTime || InKeyEvent.IsRepeat())
		return FReply::Handled();
	
	const FKey Key = InKeyEvent.GetKey();
	if (InKeyEvent.GetKey() == CloseKeyKeyboard || InKeyEvent.GetKey() == CloseKeyPad)
	{
		DeactivateWidget();
		return FReply::Handled();
	}
	if (Key == JTab -> LeftTab -> KeyboardKey || Key == JTab -> LeftTab -> GamepadKey)
	{
		JTab -> LeftTab -> PlayActionAnimation();
		return FReply::Unhandled();
	}
	if (Key == JTab -> RightTab -> KeyboardKey || Key == JTab -> RightTab -> GamepadKey)
	{
		JTab -> RightTab -> PlayActionAnimation();
		return FReply::Unhandled();
	}
	
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

