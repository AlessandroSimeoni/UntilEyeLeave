// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/RotationModifier.h"

#include "Camera/CameraComponent.h"


URotationModifier* URotationModifier::Create(UObject* Outer,
                                             const float DeltaYaw, const float DeltaPitch, const float DeltaRoll, const float Duration,UCameraComponent* Camera)
{
	TObjectPtr<URotationModifier> Modifier = NewObject<URotationModifier>(Outer);
	Modifier->RotationModifier =  FRotator(DeltaPitch, DeltaYaw, DeltaRoll);
	Modifier->SetTargetRotation(Modifier->RotationModifier);
	Modifier->InterpSpeed = Duration;
	Modifier->GameCamera = Camera;
	return Modifier;
}

void URotationModifier::Update(float DeltaTime, USpringArmComponent* SpringArm)
{
	//Spring Arm rotation
	SetRotationToTarget(DeltaTime, SpringArm);

	//camera Rotation
	if (IsValid(GameCamera))
	{
		if (IsCameraRotating)
		{
			HandleWorldCameraRotation(DeltaTime, SpringArm);
		}
		else
		{
			HandleLocalCameraRotation(DeltaTime, SpringArm);
		}
	}
}

void URotationModifier::Revert(bool Immediate)
{
	IsReverse = !IsReverse;

	if(Alpha == 1)
	{
		Alpha = 0.99;
	}
	
	if (Immediate && IsReverse)
	{
		ElapsedTime = !IsReverse;
	}
}

void URotationModifier::SetRotationToTarget(float DeltaTime, USpringArmComponent* SpringArm)
{
	FQuat CurrentQuat = SpringArm->GetComponentQuat();
	
	if (CurrentQuat.Equals(SpringArmTargetQuat, 0))
	{
		return; 
	}
	
	if (CurrentQuat.AngularDistance(SpringArmTargetQuat) < CloseEnoughThreshold)
	{
		SpringArm->SetWorldRotation(SpringArmTargetQuat);
		return;
	}
	
	FQuat SmoothedQuat = FQuat::Slerp(CurrentQuat, SpringArmTargetQuat, DeltaTime * InterpSpeed);
	
	FRotator FinalRotation = SmoothedQuat.Rotator();
	FinalRotation.Roll = 0.0f; 
	SpringArm->SetWorldRotation(FQuat(FinalRotation));
}

void URotationModifier::HandleWorldCameraRotation(float DeltaTime, USpringArmComponent* SpringArm)
{
	if (!IsValid(GameCamera)) return;

	FQuat CurrentQuat = GameCamera->GetComponentQuat();
	if (CurrentQuat.Equals(SpringArmTargetQuat, 0))
	{
		return;
	}

	if (CurrentQuat.AngularDistance(SpringArmTargetQuat) < CloseEnoughThreshold)
	{
		GameCamera->SetWorldRotation(SpringArmTargetQuat);
		return;
	}
	
		FQuat SmoothedQuat = FQuat::Slerp(CurrentQuat, CameraTargetQuat, DeltaTime * InterpSpeed);
		GameCamera->SetWorldRotation(SmoothedQuat.Rotator());
}

void URotationModifier::HandleLocalCameraRotation(float DeltaTime, USpringArmComponent* SpringArm)
{
	if (!IsValid(GameCamera)) return;
	
	FQuat CurrentQuat = GameCamera->GetRelativeRotation().Quaternion();
    
	
	FQuat TargetQuat = FQuat(FRotator::ZeroRotator);
	
	if (CurrentQuat.Equals(TargetQuat, 0.01f)) 
	{
		GameCamera->SetRelativeRotation(FRotator::ZeroRotator);
		return;
	}
	
	FQuat SmoothedQuat = FQuat::Slerp(CurrentQuat, TargetQuat, DeltaTime * InterpSpeed);
	
	GameCamera->SetRelativeRotation(SmoothedQuat.Rotator());
}

void URotationModifier::SetTargetQuat(const FQuat& TargetQuat)
{
	SpringArmTargetQuat = TargetQuat;
}
void URotationModifier::SetTargetQuat(const FQuat& TargetQuat, const float Speed)
{
	SpringArmTargetQuat = TargetQuat;
	InterpSpeed = Speed;
}
void URotationModifier::SetTargetRotation(const FRotator& TargetRotation)
{
	SpringArmTargetQuat = TargetRotation.Quaternion();
}


//Old  Rotation Function
void URotationModifier::DeltaRotation(float DeltaTime, USpringArmComponent* SpringArm)
{
	if (Alpha >= 1)
		return;
	
	if (IsReverse)
		ElapsedTime -= DeltaTime;
	else
		ElapsedTime += DeltaTime;
	
	Alpha = FMath::Clamp(ElapsedTime / InterpSpeed,0.0f,1.0f);
	

	FRotator RotationStep = RotationModifier * (Alpha - LastAlpha);
	FRotator SumRotation = (SpringArm->GetComponentRotation() + RotationStep);
	SpringArm->SetRelativeRotation(SumRotation);
	LastAlpha = Alpha;
}



bool URotationModifier::IsDestructible() const
{
	//return Alpha <= 0 && IsReverse;
	return false;
}

void URotationModifier::SetUp(const FRotator& TargetRotation, float Speed)
{
	SpringArmTargetQuat = TargetRotation.Quaternion();
	InterpSpeed = Speed;
}




