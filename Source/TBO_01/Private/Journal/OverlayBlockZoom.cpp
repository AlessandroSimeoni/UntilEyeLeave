#include "Journal/OverlayBlockZoom.h"
#include "Journal/MapWidget.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"

FReply UOverlayBlockZoom::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
	if (SaveGame -> MapFragments.Contains(1))
	{
		UE_LOG(LogTemp, Warning, TEXT("ZOOM MAP"));
		const float WheelDelta = InMouseEvent.GetWheelDelta();
		if (WheelDelta > 0)
		{
			MapWidget -> ZoomMap(+1.0f, MapWidget -> LastFocusedEntry);
		}
		if (WheelDelta < 0)
		{
			MapWidget -> ZoomMap(-1.0f, MapWidget -> LastFocusedEntry);
		}
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

FReply UOverlayBlockZoom::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}

