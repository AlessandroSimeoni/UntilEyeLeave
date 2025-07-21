// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpringArmModifier.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpringArmModifier : public UInterface
{
	GENERATED_BODY()
	
};

/**
 * 
 */
class TBO_01_API ISpringArmModifier
{
	GENERATED_BODY()
	
public:
	
	virtual void Update(float DeltaTime, USpringArmComponent* SpringArm) = 0; 
	virtual void Revert(bool Immediate) = 0;
	virtual bool IsDestructible() const = 0;
	
};
