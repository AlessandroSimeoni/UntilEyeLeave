// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Interface/SpringArmModifier.h"
#include "UObject/NoExportTypes.h"
#include "RotationModifier.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API URotationModifier : public UObject, public ISpringArmModifier
{
	GENERATED_BODY()

public:
	
	static URotationModifier* Create(UObject* Outer, float DeltaYaw, float DeltaPitch, float DeltaRoll, float Duration, UCameraComponent* Camera);
	virtual void Update(float DeltaTime, USpringArmComponent* SpringArm)override; 
	virtual void Revert(bool Immediate)override;
	virtual bool IsDestructible() const override;
	
	void SetRotationToTarget(float DeltaTime, USpringArmComponent* SpringArm);
	void HandleWorldCameraRotation(float DeltaTime, USpringArmComponent* SpringArm);
	void HandleLocalCameraRotation(float DeltaTime, USpringArmComponent* SpringArm);
	void SetTargetQuat(const FQuat& TargetQuat);
	void SetTargetQuat(const FQuat& TargetQuat, float Speed);
	void SetTargetCameraQuat(const FQuat& TargetQuat){CameraTargetQuat = TargetQuat;}
	void SetTargetRotation(const FRotator& TargetRotation);
	void SetSpeed(float Speed){InterpSpeed = Speed;}
	void SetUp(const FRotator& TargetRotation, float Speed);

	UCameraComponent* GetCameraComponent(){return GameCamera;}
	bool IsCameraRotating = false;

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> GameCamera;
	
	/** Old Function to handle rotation*/
	void DeltaRotation(float DeltaTime, USpringArmComponent* SpringArm);
	
	FVector TargetSpringArmWorldRotation;
	FRotator StartRotation;
	FRotator RotationModifier;
	FQuat SpringArmTargetQuat;
	FQuat CameraTargetQuat = FRotator::ZeroRotator.Quaternion();;
	const float RotationThreshold = 0.001f;
	const float CloseEnoughThreshold = 0.001f; 

	
	float InterpSpeed;
	
	//0 start 1 complete
	float Alpha = 0;
	float LastAlpha = 0;
	float ElapsedTime=0;

	bool IsReverse = false;
	
	
	
	
};

