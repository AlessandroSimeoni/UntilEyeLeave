#include "MainMenu/MainMenuWidget.h"
#include "MainMenu/MainMenuButton.h"
#include "SaveSystem/SaveSubsystem.h"


void UMainMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	LastClickedButton = nullptr;
	if (ContinueButton -> IsFocusable())
	{
		ContinueButton -> OnButtonSelection.AddDynamic(this, &UMainMenuWidget::StartLerp);
		ContinueButton -> OnButtonClicked.AddDynamic(this, &UMainMenuWidget::SetLastClick);
	}
	NewGameButton -> OnButtonSelection.AddDynamic(this, &UMainMenuWidget::StartLerp);
	NewGameButton -> OnButtonClicked.AddDynamic(this, &UMainMenuWidget::SetLastClick);
	OptionsButton -> OnButtonSelection.AddDynamic(this, &UMainMenuWidget::StartLerp);
	OptionsButton -> OnButtonClicked.AddDynamic(this, &UMainMenuWidget::SetLastClick);
	CreditsButton -> OnButtonSelection.AddDynamic(this, &UMainMenuWidget::StartLerp);
	CreditsButton -> OnButtonClicked.AddDynamic(this, &UMainMenuWidget::SetLastClick);
	QuitButton -> OnButtonSelection.AddDynamic(this, &UMainMenuWidget::StartLerp);
	QuitButton -> OnButtonClicked.AddDynamic(this, &UMainMenuWidget::SetLastClick);
}

void UMainMenuWidget::StartLerp(ACameraActor* Camera)
{
	OnLerpRequest.Broadcast(Camera);
}

void UMainMenuWidget::SetLastClick(UMainMenuButton* Button)
{
	LastClickedButton = Button;
}

void UMainMenuWidget::ResetLastClickedButton()
{
	LastClickedButton = nullptr;
}

void UMainMenuWidget::InitialButtonFocus()
{
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!SaveSubsystem -> DoesSaveExist())
	{
		ContinueButton ->SetIsFocusable(false);
		ContinueButton ->SetIsSelectable(false);
		ContinueButton ->SetIsEnabled(false);
		NewGameButton -> SetFocus();
	}
	else
	{
		//ContinueButton ->SetIsFocusable(true);
		//ContinueButton ->SetIsSelectable(true);
		ContinueButton ->SetIsEnabled(true);
		ContinueButton ->SetFocus();
	}
}

