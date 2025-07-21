// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterAnimInstance.h"

#include "Character/MainCharacter.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerPawn = TryGetPawnOwner();
	if(OwnerPawn)
		OwnerCharacter = Cast<AMainCharacter>(OwnerPawn);
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(IdleToRunLerp)
	{
		IdleToRunRatio = FMath::FInterpTo(IdleToRunRatio, TargetIdleToRunValue, DeltaSeconds, IdleToRunLerpSpeed);

		if (FMath::IsNearlyEqual(IdleToRunRatio, TargetIdleToRunValue, 0.001f))
		{
			IdleToRunRatio = TargetIdleToRunValue;
			IdleToRunLerp = false;
		}
	}
}

void UCharacterAnimInstance::StartIdleToRunInterpolation(float TargetValue)
{
	IdleToRunLerp = false;
	TargetIdleToRunValue = TargetValue;
	IdleToRunLerp = true;
}


