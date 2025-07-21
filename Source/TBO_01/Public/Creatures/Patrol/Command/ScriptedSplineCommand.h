// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/Patrol/Interface/SplineBehaviorCommand.h"
#include "UObject/Object.h"
#include "ScriptedSplineCommand.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UScriptedSplineCommand : public UObject,  public ISplineBehaviorCommand
{
	GENERATED_BODY()
	virtual FVector Execute(USplineComponent* Spline, int32& Index) override;
	virtual void IncrementIndex(class USplineComponent*  Spline, int32& Index) override;	
};
