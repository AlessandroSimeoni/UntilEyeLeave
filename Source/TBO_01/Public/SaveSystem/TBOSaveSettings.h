// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TBOSaveSettings.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UTBOSaveSettings : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float MasterVolume = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float MusicVolume = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float EffectsVolume = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float InterfaceVolume = 1.0f;
};
