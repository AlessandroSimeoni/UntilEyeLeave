// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Camera/GameCamera.h"
#include "UObject/Interface.h"
#include "TargetCameraActor.generated.h"

// This class does not need to be modified.

//Used on actor that the camera can attach to
//Used when the camera attach to an actor

UINTERFACE(MinimalAPI)
class UTargetCameraActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TBO_01_API ITargetCameraActor
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "My Interface")
	void SetCamera(AGameCamera* Camera);
};
