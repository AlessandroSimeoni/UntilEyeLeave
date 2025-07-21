// Fill out your copyright notice in the Description page of Project Settings.
#include "Creatures/Action/ReportNoiseComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Creatures/CreatureEnumHelper.h"
#include "Perception/AISense_Touch.h"


// Sets default values for this component's properties
UReportNoiseComponent::UReportNoiseComponent()
{
	
	//PrimaryComponentTick.bCanEverTick = true;
	
}

void UReportNoiseComponent::MakeNoiseDefault()
{
	
	FName Tag =  UCreatureEnumHelper::EnumToFName<ENoiseAction>(SoundType);
	UAISense_Hearing::ReportNoiseEvent(GetOwner(),GetOwner()->GetActorLocation(),Loudness,GetOwner(),MaxRange,Tag);
	DrawNoiseDebug(GetOwner()->GetActorLocation(), MaxRange);
}

void UReportNoiseComponent::MakeNoise(ENoiseAction SoundTypeValue, float LoudnessValue, float MaxRangeValue)
{
	FName Tag =  UCreatureEnumHelper::EnumToFName<ENoiseAction>(SoundTypeValue);
	UAISense_Hearing::ReportNoiseEvent(GetOwner(),GetOwner()->GetActorLocation(),LoudnessValue,GetOwner(),MaxRangeValue,Tag);
	DrawNoiseDebug(GetOwner()->GetActorLocation(), MaxRange);
	
}


void UReportNoiseComponent::DrawNoiseDebug(FVector Location, float Radius)
{
	if (!DrawDebug) return;
	FColor DebugColor = FColor::Green;
	
	DrawDebugCircle(GetWorld(), Location, Radius, 50, DebugColor, false, 2.0f, 0, 2.0f, FVector(1, 0, 0), FVector(0, 1, 0));
}




