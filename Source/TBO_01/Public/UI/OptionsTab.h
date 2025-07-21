// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "OptionsTab.generated.h"


class UActionWidget;
/**
 * 
 */
UCLASS()
class TBO_01_API UOptionsTab : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "Actions")
	UActionWidget* LeftTab;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "Actions")
	UActionWidget* RightTab;
};
