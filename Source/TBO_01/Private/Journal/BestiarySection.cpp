// Fill out your copyright notice in the Description page of Project Settings.


#include "Journal/BestiarySection.h"
#include "Journal/ActionWidget.h"
#include "Journal/EntryJournalButton.h"

void UBestiarySection::AddToBestiaryButtons(UEntryJournalButton* Button)
{
	BestiaryButtons.Add(Button);
}

void UBestiarySection::NativeOnActivated()
{
	BestiaryButtons.Empty();
	FocusIndex = 0;
	Super::NativeOnActivated();
}

FReply UBestiarySection::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey == EKeys::W || PressedKey == EKeys::S)
	{
		int32 NumButtons = BestiaryButtons.Num();
		UE_LOG(LogTemp, Warning, TEXT("AvailableButtons: %d"), NumButtons);
		if (NumButtons == 0)
		{
			return FReply::Unhandled();
		}

		int32 Direction = (PressedKey == EKeys::W) ? -1 : 1;
		FocusIndex = (FocusIndex + Direction + NumButtons) % NumButtons;
		UE_LOG(LogTemp, Warning, TEXT("Focus Index: %d"), FocusIndex);
		BestiaryButtons[FocusIndex] -> SetFocus();
		BestiaryButtons[FocusIndex] -> SetIsSelected(true);

		return FReply::Handled();
	}
	if (PressedKey == EKeys::Up || PressedKey == EKeys::Down)
	{
		UE_LOG(LogTemp, Warning, TEXT("Intercepted"));
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UBestiarySection::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == UpAction -> KeyboardKey || Key == UpAction ->GamepadKey || Key == EKeys::Up)
	{
		UpAction -> PlayActionAnimation();
		return FReply::Unhandled();
	}
	if (Key == DownAction -> KeyboardKey || Key == DownAction ->GamepadKey || Key == EKeys::Down)
	{
		DownAction -> PlayActionAnimation();
		return FReply::Unhandled();
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UBestiarySection::SetFocusIndex(int32 Index)
{
	FocusIndex = Index;
}

