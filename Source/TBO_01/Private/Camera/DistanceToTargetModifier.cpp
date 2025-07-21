// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/DistanceToTargetModifier.h"



UDistanceToTargetModifier* UDistanceToTargetModifier::Create(UObject* Outer, AActor* MainActor, AActor *TargetActor,
															 UCurveFloat* Curve,float DeltaLength,float StartLenght, ULenghtModifier* SpringArmLenghtModifier)
{
	
	TObjectPtr<UDistanceToTargetModifier> Modifier = NewObject<UDistanceToTargetModifier>(Outer);
	Modifier->MainActor = MainActor;
	Modifier->TargetActor = TargetActor;
	Modifier->Curve = Curve;
	Modifier->DeltaLenght = DeltaLength;
	Modifier->StartActorDistance = Modifier->CalculateDistance();
	Modifier->StartLenght = StartLenght;
	Modifier->LenghtModifier = SpringArmLenghtModifier;
	return Modifier;
}

void UDistanceToTargetModifier::Update(float DeltaTime, USpringArmComponent* SpringArm)
{
	if (!MainActor || !TargetActor || !Curve || !SpringArm)
	{
		return;
	}

	if (IsEnded)
	{
		return;
	}

	float CurrentDistance = CalculateDistance();
	Alpha = 1 - FMath::Clamp(CurrentDistance / StartActorDistance, 0.0f, 1.0f);
	float CurveValue = Curve->GetFloatValue(Alpha);
	float ArmLenghtToReach = StartLenght + (DeltaLenght * CurveValue);
	LenghtModifier->SetLenght(ArmLenghtToReach);
	//SpringArm->TargetArmLength += (DeltaLength * (CurveValue - LastAlpha)); 
	//LastAlpha = CurveValue;
	
}

void UDistanceToTargetModifier::Revert(bool Immediate)
{
	IsEnded = true;
	if (Immediate)
		Alpha = 0;
}

bool UDistanceToTargetModifier::IsDestructible() const
{
	return IsEnded;
}

float UDistanceToTargetModifier::CalculateDistance() const
{
	if (!IsValid(MainActor) || !IsValid(TargetActor)) 
		return 0.f;
	return MainActor->GetSquaredHorizontalDistanceTo(TargetActor);
}

void  UDistanceToTargetModifier::SetUp(AActor *Actor,UCurveFloat* AlphaCurve,float DeltaArmLength,float StartArmLenght)
{
	TargetActor = Actor;
	Curve = AlphaCurve;
	DeltaLenght = DeltaArmLength;
	StartActorDistance = CalculateDistance();
	StartLenght = StartArmLenght;
}