// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EyesHandlerActorComponent.h"
#include "UObject/Interface.h"
#include "EyesHandlerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEyesHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TBO_01_API IEyesHandlerInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="TBO Interface")
	UEyesHandlerActorComponent* GetEyesHandlerActorComponent();
};
