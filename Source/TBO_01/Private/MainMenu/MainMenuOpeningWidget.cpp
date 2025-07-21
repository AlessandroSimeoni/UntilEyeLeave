#include "MainMenu/MainMenuOpeningWidget.h"

FReply UMainMenuOpeningWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	AnyKeyPressedEvent.Broadcast();
	return FReply::Handled();
	//return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UMainMenuOpeningWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	AnyKeyPressedEvent.Broadcast();
	return FReply::Handled();
	//return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
