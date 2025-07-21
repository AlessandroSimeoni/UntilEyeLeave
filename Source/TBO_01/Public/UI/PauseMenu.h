// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UPauseMenu : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PauseMenu", Meta = (AllowPrivateAccess = true, UIMin = 0.0f, ClampMin = 0.0f))
	float CloseCooldownTime = 0.5f;

	float OpenedTime = 0.0f;
	
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
