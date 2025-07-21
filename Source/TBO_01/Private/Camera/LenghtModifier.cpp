// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LenghtModifier.h"

ULenghtModifier* ULenghtModifier::Create(UObject* Outer, float DeltaLenght, float Speed)
{
	TObjectPtr<ULenghtModifier> Modifier = NewObject<ULenghtModifier>(Outer);
	Modifier->TargetSpringArmLenght = DeltaLenght;
	Modifier->InterpSpeed = Speed;
	return Modifier;
}

void ULenghtModifier::Update(float DeltaTime, USpringArmComponent* SpringArm)
{
	TargetLenght(DeltaTime, SpringArm);
	OnLengthUpdate.Broadcast(SpringArm->TargetArmLength);
}

void ULenghtModifier::TargetLenght(float DeltaTime, USpringArmComponent* SpringArm)
{
	if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, TargetSpringArmLenght, LengthThreshold))
	{
		return;  
	}
	
	if (FMath::Abs(SpringArm->TargetArmLength - TargetSpringArmLenght) < CloseEnoughThreshold)
	{
		SpringArm->TargetArmLength = TargetSpringArmLenght;
		return;
	}

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetSpringArmLenght, DeltaTime, InterpSpeed);
}


//old Method
void ULenghtModifier::DeltaLenght(float DeltaTime, USpringArmComponent* SpringArm)
{
	if (Alpha>= 1)
		return;
	if (IsReverting)
		ElapsedTime -= DeltaTime;
	else
		ElapsedTime += DeltaTime;
	
	Alpha = FMath::Clamp(ElapsedTime/InterpSpeed, 0, 1);
	SpringArm->TargetArmLength = SpringArm->TargetArmLength + TargetSpringArmLenght * (Alpha - LastAlpha);
	LastAlpha = Alpha;
}



void ULenghtModifier::Revert(bool Immediate)
{
	IsReverting = !IsReverting;

	if (Alpha == 1)
	{
		Alpha = 0.99f;
	}
	if (Immediate && IsReverting)
	{
		ElapsedTime = !IsReverting;
	}
}

bool ULenghtModifier::IsDestructible() const
{
	return false;
	//return IsReverting && Alpha <= 0;
}

void ULenghtModifier::SetUp(float TargetLenght,float Speed)
{
	TargetSpringArmLenght = TargetLenght;
	InterpSpeed = Speed;
}


