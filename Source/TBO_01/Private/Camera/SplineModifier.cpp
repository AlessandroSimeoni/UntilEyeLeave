// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/SplineModifier.h"
#include "Camera/CameraComponent.h"

USplineModifier* USplineModifier::Create(UObject *Outer, USplineComponent* Spline, URotationModifier* RotationModifier, ULenghtModifier* LengthModifier)
{
	TObjectPtr<USplineModifier> SplineModifier = NewObject<USplineModifier>(Outer);
	SplineModifier->SplineComponent = Spline;
	SplineModifier->RotationModifier = RotationModifier;
	SplineModifier->LenghtModifier = LengthModifier;
	return SplineModifier;
}

void USplineModifier::Update(float DeltaTime, USpringArmComponent* SpringArm)
{
	
	if (!IsValid(SplineComponent)||!IsValid(LenghtModifier)||!IsValid(RotationModifier)||IsEnded) return;

	FVector PlayerLocation = SpringArm->GetAttachParent()->GetComponentLocation();
	if (!IsUsingStartEndPoint)
	{
		float NearestDistance = SplineComponent->FindInputKeyClosestToWorldLocation(PlayerLocation);
		TargetDistanceAlongSpline = SplineComponent->GetDistanceAlongSplineAtSplineInputKey(NearestDistance);
	}
	else
	{
		float Distance = FVector::DistSquared(PlayerLocation,EndPoint);
		TargetDistanceAlongSpline =  FMath::Clamp(1-(Distance / StartToEndDistance), 0.0f, 1.0f) * SplineComponent->GetSplineLength();
		
	}
	

	if (CurrentDistanceAlongSpline == 0)
	{
		CurrentDistanceAlongSpline = TargetDistanceAlongSpline;
	}
	
	CurrentDistanceAlongSpline = FMath::FInterpTo(CurrentDistanceAlongSpline, TargetDistanceAlongSpline, DeltaTime, SpeedAlongSpline);
	FVector ClosestPointOnSpline = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
	
	FVector DirectionToSplinePoint = (ClosestPointOnSpline - PlayerLocation).GetSafeNormal();

	DrawDebugLine(
	GetWorld(),                 
	PlayerLocation,               
	PlayerLocation + (DirectionToSplinePoint * 600.0f),
	FColor::Green,                
	false,                        
	0.0f,                         
	0,                            
	2.0f                          
);
	//set arm lenght
	if (IsAdaptingSpringArmLength)
	{
		float TargetArmLength = FVector::Dist(PlayerLocation, ClosestPointOnSpline);
		LenghtModifier->SetUp(TargetArmLength,SpringArmLenghtSpeed);
	}
	
	//set spring arm rotation
	FQuat TargetQuat = FRotationMatrix::MakeFromX(DirectionToSplinePoint).ToQuat() * FQuat(FRotator(0.f, 180.f, 0.f));
	RotationModifier->SetTargetQuat(TargetQuat,SpringArmRotationSpeed);


	//Set camera component rotation
	if (!IsDolly)
		return;
	
	UCameraComponent* Camera = RotationModifier->GetCameraComponent();
	if (IsValid(Camera))
	{
		FVector SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
		FVector SplineRightVector = SplineComponent->GetRightVectorAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
		
		FVector ToPlayerDirection = (PlayerLocation - SplineLocation).GetSafeNormal();
		FRotator TargetRotation;
		if (IsDollyAligned)
		{
			float DotProduct = FVector::DotProduct(SplineRightVector, ToPlayerDirection);
			FVector CameraDirection = (DotProduct > 0.0f) ? SplineRightVector : -SplineRightVector;
			CameraDirection.Normalize();
			TargetRotation = CameraDirection.Rotation();
		}
		else if (IsForwardAligned)
		{
			FVector CameraDirection = SplineComponent->GetDirectionAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
			CameraDirection.Normalize();
			TargetRotation = CameraDirection.Rotation();
		}
		else
		{
			ToPlayerDirection.Z = 0.0f; 
			ToPlayerDirection.Normalize();
			TargetRotation = ToPlayerDirection.Rotation();
		}
		TargetRotation.Pitch = 0.0f;  
		TargetRotation.Roll = 0.0f;
		
		FQuat TargetCameraQuat = TargetRotation.Quaternion();
		RotationModifier->SetTargetCameraQuat(TargetCameraQuat);
	}
	
}

void USplineModifier::Revert(bool Immediate)
{
	
}

bool USplineModifier::IsDestructible() const
{
	return false;
}

void USplineModifier::SetUp(USplineComponent* Spline,const FVector& Start, const FVector& End, const bool UseStartEndPointDistance, const bool AdaptSpringArmLenght,const bool Dolly, const bool DollyAligned,const bool ForwardAligned , float RotationSpeed, float LenghtSpeed, float SplineSpeed)
{
	StartToEndDistance = FVector::Distance(Start,End);
	SplineComponent = Spline;
	StartToEndDistance *= StartToEndDistance;
	SpringArmRotationSpeed = RotationSpeed;
	SpringArmLenghtSpeed = LenghtSpeed;
	SpeedAlongSpline = SplineSpeed;
	EndPoint = End;
	CurrentDistanceAlongSpline = 0;
	IsUsingStartEndPoint = UseStartEndPointDistance;
	IsAdaptingSpringArmLength = AdaptSpringArmLenght;
	IsDolly = Dolly;
	IsDollyAligned = DollyAligned;
	IsForwardAligned = ForwardAligned;
}
