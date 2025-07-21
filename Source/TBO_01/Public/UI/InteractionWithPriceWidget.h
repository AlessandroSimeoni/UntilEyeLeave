// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActionWidget.h"
#include "CommonActivatableWidget.h"
#include "InteractionWithPriceWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UImage;

UCLASS(Blueprintable)
class TBO_01_API UInteractionWithPriceWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetupInteraction(float Price);

	UPROPERTY(meta = (BindWidget))
	UCommonActionWidget* ActionWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PriceText;
	

	UPROPERTY(meta = (BindWidget))
	UImage* PriceIcon;

};
