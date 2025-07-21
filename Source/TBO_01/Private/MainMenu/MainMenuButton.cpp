#include "MainMenu/MainMenuButton.h"

void UMainMenuButton::SetBindCameraActor(class ACameraActor* CameraActor)
{
	BindCamera =CameraActor;
}

void UMainMenuButton::HandleSelection()
{
	OnButtonSelection.Broadcast(BindCamera);
}

void UMainMenuButton::HandleClicked()
{
	OnButtonClicked.Broadcast(this);
}
