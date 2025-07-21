// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/OffsetToForwardModifier.h"

UOffsetToForwardModifier* UOffsetToForwardModifier::Create(UObject* Outer,AActor* Target, const float OffsetValue, float InterpSpeed = 2)
{
	TObjectPtr<UOffsetToForwardModifier> Modifier = NewObject<UOffsetToForwardModifier>(Outer); 
	Modifier->SetParameter(Target,OffsetValue,InterpSpeed);
	return Modifier;
}

void UOffsetToForwardModifier::Update(const float DeltaTime, USpringArmComponent* SpringArm)
{
	//FVector OffsetDistance = Target->GetActorForwardVector() * Offset;
	//SpringArm->TargetOffset = FMath::VInterpTo(SpringArm->TargetOffset,OffsetDistance,DeltaTime,InterpSpeed);
	if (!Target || !SpringArm) return;
	
	FVector CameraLocation = SpringArm->GetSocketLocation(SpringArm->SocketName);
	CameraLocation += SpringArm->GetForwardVector() * ForwardOffset;
	FVector RightVector = SpringArm->GetRightVector();
	FVector LeftRayEnd = CameraLocation - (RightVector * RayDistance);
	FVector RightRayEnd = CameraLocation + (RightVector * RayDistance);
	
	bool bLeftCollision = GetWorld()->SweepSingleByChannel(
		LeftHit, CameraLocation, LeftRayEnd, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(SphereRadius)
	);

	bool bRightCollision = GetWorld()->SweepSingleByChannel(
		RightHit, CameraLocation, RightRayEnd, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(SphereRadius)
	);
	
	//DrawDebugSphere(GetWorld(), LeftRayEnd, SphereRadius, 12, FColor::Red, false, 0.1f);
	//DrawDebugSphere(GetWorld(), RightRayEnd, SphereRadius, 12, FColor::Blue, false, 0.1f);
	
	FVector OffsetAdjustment = FVector::ZeroVector;

	float AppliedInterpSpeed = InterpSpeed;
	if (bLeftCollision)
	{
		OffsetAdjustment += RightVector * CollisionAvoidanceOffset;
		AppliedInterpSpeed += 7.0f;
	}

	if (bRightCollision)
	{
		OffsetAdjustment -= RightVector * CollisionAvoidanceOffset;
		AppliedInterpSpeed += 7.0f;
	}
	
	FVector OffsetDistance = (Target->GetActorForwardVector() * Offset) + OffsetAdjustment;
	SpringArm->TargetOffset = FMath::VInterpTo(SpringArm->TargetOffset, OffsetDistance, DeltaTime, AppliedInterpSpeed);
}

void UOffsetToForwardModifier::Revert(bool Immediate)
{
	IsReverting = !IsReverting;
	Offset = StartOffsetValue * IsReverting;
}

bool UOffsetToForwardModifier::IsDestructible() const
{
	return false;
}

void UOffsetToForwardModifier::SetParameter(AActor* InTarget, float InOffsetValue, float InInterpSpeed)
{
	Target = InTarget; 
	StartOffsetValue = Offset = InOffsetValue;
	InterpSpeed = InInterpSpeed;
}

void UOffsetToForwardModifier::SetTargetActor(AActor* NewTarget) {Target = NewTarget;}

bool UOffsetToForwardModifier::TargetIsNull() const { return Target == nullptr; }

void UOffsetToForwardModifier::SetOffset(float NewOffset){Offset = NewOffset;}

