// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "SkipCutsceneWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkipCutsceneEvent);

UCLASS()
class TBO_01_API USkipCutsceneWidget : public UNewBaseWidgetClass
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle SkipCutsceneActionData;

	FUIActionBindingHandle SkipCutsceneHandle;

	void HandleSkipCutsceneAction();
	virtual void NativeOnInitialized() override;

public:
	FOnSkipCutsceneEvent OnSkipCutsceneEvent;
};
