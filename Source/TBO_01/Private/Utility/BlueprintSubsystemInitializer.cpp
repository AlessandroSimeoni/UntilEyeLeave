// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/BlueprintSubsystemInitializer.h"

void UGameInstanceSubsystemInitializer::Initialize(FSubsystemCollectionBase& Collection)
{
	//Super::Initialize(Collection);
	const UBlueprintSubsystemInitializer* BlueprintSubsystemInitializer = GetDefault<UBlueprintSubsystemInitializer>();

	if (BlueprintSubsystemInitializer->GameInstanceSubsystems.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstanceSubsystemInitializer is empty"));
		return;
	}
	for (TSubclassOf<UGameInstanceSubsystem> Subsystem: BlueprintSubsystemInitializer->GameInstanceSubsystems)
	{
		Collection.InitializeDependency(Subsystem);
	}
}
