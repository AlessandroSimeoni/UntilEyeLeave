// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "MapIconsLegend.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UMapIconsLegend : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UCommonRichTextBlock* IconName;
	
};
