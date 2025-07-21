// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActionableActor.h"
#include "Portal.generated.h"

UCLASS()
class TBO_01_API APortal : public AActionableActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnPortal();

	virtual void ActivateActor_Implementation(bool InstantActivation) override;
};
