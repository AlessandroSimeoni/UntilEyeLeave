// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SpringArmModifier.h"
#include "UObject/NoExportTypes.h"
#include "LenghtModifier.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLengthUpdate, float, NewLength);

UCLASS()
class TBO_01_API ULenghtModifier : public UObject,public  ISpringArmModifier
{
	GENERATED_BODY()

public:
	static ULenghtModifier* Create(UObject* Outer,float DeltaLenght,float Speed);
	virtual void Update(float DeltaTime, USpringArmComponent* SpringArm) override;
	void TargetLenght(float DeltaTime, USpringArmComponent* SpringArm);
	void DeltaLenght(float DeltaTime, USpringArmComponent* SpringArm);
	virtual void Revert(bool Immediate) override;
	virtual bool IsDestructible() const override;
	
	void SetLenght(float TargetLenght){TargetSpringArmLenght = TargetLenght;}
	float GetTargetArmLenght() const {return TargetSpringArmLenght;} 
	void SetSpeed(float Speed){InterpSpeed = Speed;}
	
	void SetUp(float TargetLenght, float Speed);

	FOnLengthUpdate OnLengthUpdate;
	
protected:
	const float LengthThreshold = 0.1f;  
	const float CloseEnoughThreshold = 0.2f;
	float InterpSpeed;
	float TargetSpringArmLenght;
	float Alpha = 0;
	float LastAlpha = 0;
	float ElapsedTime=0;
	bool IsReverting = false;
};
