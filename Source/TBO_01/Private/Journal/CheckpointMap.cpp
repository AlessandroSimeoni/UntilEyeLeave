// Fill out your copyright notice in the Description page of Project Settings.


#include "Journal/CheckpointMap.h"

void UCheckpointMap::NativeOnActivated()
{
	Super::NativeOnActivated();
	OpenedTime = GetWorld()->GetRealTimeSeconds();
}

FReply UCheckpointMap::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const float Now = GetWorld()->GetRealTimeSeconds();
	const float TimeSinceOpen = Now - OpenedTime;

	if (TimeSinceOpen < CloseCooldownTime || InKeyEvent.IsRepeat())
		return FReply::Handled();
	
	const FKey Key = InKeyEvent.GetKey();
	if (InKeyEvent.GetKey() == EKeys::Tab || InKeyEvent.GetKey() == EKeys::Escape)
	{
		DeactivateWidget();
		return FReply::Handled();
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

