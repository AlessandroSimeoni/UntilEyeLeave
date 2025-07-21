// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkipCutsceneWidget.h"

#include "Input/CommonUIInputTypes.h"


void USkipCutsceneWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetIsFocusable(true);
	SkipCutsceneHandle = RegisterUIActionBinding(FBindUIActionArgs(SkipCutsceneActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleSkipCutsceneAction)));
}

void USkipCutsceneWidget::HandleSkipCutsceneAction()
{
	OnSkipCutsceneEvent.Broadcast();
}
