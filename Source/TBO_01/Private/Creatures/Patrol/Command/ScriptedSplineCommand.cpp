// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Patrol/Command/ScriptedSplineCommand.h"

#include "Components/SplineComponent.h"

FVector UScriptedSplineCommand::Execute(USplineComponent* Spline, int32& Index)
{
	if (Index < Spline->GetNumberOfSplinePoints()-1)
	{
		FVector WorldPosition = Spline->GetLocationAtSplinePoint(Index,ESplineCoordinateSpace::Local);
		return WorldPosition;
	}
	else
	{
		FVector WorldPosition = Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints()-1,ESplineCoordinateSpace::Local);
		return WorldPosition;
	}
}

void UScriptedSplineCommand::IncrementIndex(class USplineComponent*  Spline, int32& Index)
{
	if (Index < Spline->GetNumberOfSplinePoints())
	{
		Index++;
	}
}
