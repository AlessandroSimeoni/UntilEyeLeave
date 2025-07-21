#include "Journal/CheckpointListEntry.h"
#include "CommonTextBlock.h"
#include "Journal/MapWidget.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"

void UCheckpointListEntry::HandleClicked()
{
	OnCheckpointEntryClicked.Broadcast(this);
}

void UCheckpointListEntry::HandleFocused()
{
	MapWidget ->SetFocusIndex(IndexValue);
	OnCheckpointEntryFocused.Broadcast(this);
}

void UCheckpointListEntry::HandleUnFocused()
{
	OnCheckpointEntryUnFocused.Broadcast(this);
}

FReply UCheckpointListEntry::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
	if (SaveGame -> MapFragments.Contains(1))
	{
		const FKey Key = InKeyEvent.GetKey();
		if (Key == EKeys::Gamepad_RightTrigger)
		{
			MapWidget -> ZoomMap(1.0f, this);
			return FReply::Handled();
		}
		if (Key == EKeys::Gamepad_LeftTrigger)
		{
			MapWidget -> ZoomMap(-1.0f, this);
			return FReply::Handled();
		}
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

/*FReply UCheckpointListEntry::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
	if (SaveGame -> MapFragments.Contains(1))
	{
		if(CheckpointName->GetText().ToString() != TEXT("Undiscovered"))
		{
			const float WheelDelta = InMouseEvent.GetWheelDelta();
			if (WheelDelta > 0)
			{
				MapWidget -> ZoomMap(+1.0f, this);
			}
			if (WheelDelta < 0)
			{
				MapWidget -> ZoomMap(-1.0f, this);
			}
		}
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}*/

