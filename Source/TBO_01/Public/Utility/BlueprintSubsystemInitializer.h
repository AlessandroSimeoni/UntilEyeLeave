// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BlueprintSubsystemInitializer.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig ,meta = (DisplayName = "Blueprint Subsystem Initializer"))
class TBO_01_API UBlueprintSubsystemInitializer : public UDeveloperSettings
{
	GENERATED_BODY()
	public:

UPROPERTY(config, EditAnywhere,Meta = (BlueprintBaseOnly))
TArray<TSubclassOf<UGameInstanceSubsystem>> GameInstanceSubsystems;

};

UCLASS()
class TBO_01_API UGameInstanceSubsystemInitializer : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;
	
};
