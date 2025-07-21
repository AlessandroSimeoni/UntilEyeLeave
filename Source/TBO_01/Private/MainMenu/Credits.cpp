#include "MainMenu/Credits.h"

void UCredits::NativeOnActivated()
{
	Super::NativeOnActivated();
	OpenedTime = GetWorld()->GetRealTimeSeconds();
}

FReply UCredits::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const float Now = GetWorld()->GetRealTimeSeconds();
	const float TimeSinceOpen = Now - OpenedTime;

	if (TimeSinceOpen < CloseCooldownTime || InKeyEvent.IsRepeat())
		return FReply::Handled();
	
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::X || Key == EKeys::Gamepad_FaceButton_Right)
	{
		NativeOnHandleBackAction(); // close the pause menu
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
