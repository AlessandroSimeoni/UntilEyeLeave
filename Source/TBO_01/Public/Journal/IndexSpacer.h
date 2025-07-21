// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IndexSpacer.generated.h"

class USpacer;

UCLASS()
class TBO_01_API UIndexSpacer : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,meta = (BindWidget))
	USpacer* Spacer;
	
};
