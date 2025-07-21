// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "CheckpointMap.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UCheckpointMap : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	float CloseCooldownTime = 1.0f;
private:
	float OpenedTime;
public:
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
