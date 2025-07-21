// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Patrol/ScriptedPatrolPath.h"

#include "Abilities/GameplayAbility.h"


FScriptedAction AScriptedPatrolPath::GetAction(int Position)
{
	if (!Spline || !Spline->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spline component is missing or invalid."));
		return FScriptedAction();
	}

	const int32 SplinePoints = Spline->GetNumberOfSplinePoints();

	if (Position < 0 || Position >= SplinePoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Requested position %d is out of spline bounds (max: %d)."), Position, SplinePoints - 1);
		return FScriptedAction();
	}

	if (!ScriptedActions.IsValidIndex(Position))
	{
		UE_LOG(LogTemp, Warning, TEXT("No scripted action defined for spline point %d."), Position);
		return FScriptedAction();
	}

	return ScriptedActions[Position];
}




#if WITH_EDITOR
void AScriptedPatrolPath::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (Spline)
	{
		const int32 SplinePoints = Spline->GetNumberOfSplinePoints();
		ScriptedActions.SetNum(SplinePoints);
	}
}
#endif