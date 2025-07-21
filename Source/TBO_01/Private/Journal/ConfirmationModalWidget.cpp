#include "Journal/ConfirmationModalWidget.h"
#include "CommonButtonBase.h"
#include "Journal/CheckpointMapButton.h"


void UConfirmationModalWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UConfirmationModalWidget::HandleConfirmClicked()
{
	OnConfirmed.Broadcast();
	ActivatorButton -> TeleportPlayer();
	DeactivateWidget();
}

void UConfirmationModalWidget::HandleCancelClicked()
{
	OnCanceled.Broadcast();
	DeactivateWidget();
}

void UConfirmationModalWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	CancelButton -> SetFocus();
	CancelButton -> SetIsSelected(true);
}

