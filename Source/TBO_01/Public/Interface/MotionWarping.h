// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MotionWarping.generated.h"

class UMotionWarpingComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMotionWarping : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TBO_01_API IMotionWarping
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "My Interface")
	UMotionWarpingComponent* GetMotionWarpingComponent();
};
