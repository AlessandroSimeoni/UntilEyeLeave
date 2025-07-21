// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LockOnModifier.h"

ULockOnModifier* ULockOnModifier::Create(UObject* Outer, AActor* MainActor, AActor* TargetActor, float Offset,
	float InterpSpeed, float MinDistance,float MaxDistance )
{
	TObjectPtr<ULockOnModifier> Modifier = NewObject<ULockOnModifier>(Outer);
	Modifier->MainActor = MainActor;
	Modifier->TargetActor = TargetActor;
	Modifier->Offset = Offset;
	Modifier->InterpSpeed = InterpSpeed;
	Modifier->SetMinMaxDistance(MinDistance,MaxDistance);
	return  Modifier;
}

void ULockOnModifier::Update(float DeltaTime, USpringArmComponent* SpringArm)
{
	if (TargetActor == nullptr)
		return;
	
	FVector MainLocation = MainActor->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	float Distance = MainActor->GetSquaredHorizontalDistanceTo(TargetActor);
	float OffsetFactor = FMath::Clamp((Distance - MinDistance) / (MaxDistance - MinDistance), 0.0f, 1.0f);


	
	FVector DirectionToTarget = (TargetLocation - MainLocation);
	DirectionToTarget.Z = 0.0f; 
	DirectionToTarget = DirectionToTarget.GetSafeNormal();

	
	
	FVector Forward = SpringArm->GetForwardVector(); 
	FVector Right = SpringArm->GetRightVector();
	
	float OffsetX = FVector::DotProduct(DirectionToTarget, Forward); 
	float OffsetY = FVector::DotProduct(DirectionToTarget, Right);

	
	FVector TargetOffset = FVector(OffsetX, OffsetY, 0.0f) * (Offset * OffsetFactor) ;

	SpringArm->TargetOffset = FMath::VInterpTo(SpringArm->TargetOffset, TargetOffset, DeltaTime, InterpSpeed);
	
}

void ULockOnModifier::Revert(bool Immediate)
{
	TargetActor = nullptr;
}

bool ULockOnModifier::IsDestructible() const
{
	return  false;
}


void ULockOnModifier::SetMinMaxDistance(float Min, float Max)
{
	//distance is calculated with squared soo min and max are converted
	MinDistance = FMath::Square(Min);
	MaxDistance = FMath::Square(Max);
}

AActor* ULockOnModifier::GetTargetActor() { return TargetActor; }
	

void ULockOnModifier::SetTargetActor(AActor* NewTarget) {TargetActor = NewTarget;}


bool ULockOnModifier::TargetIsNull() const{ return TargetActor == nullptr; }
