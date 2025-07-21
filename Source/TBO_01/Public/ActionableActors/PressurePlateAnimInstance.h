// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PressurePlateAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UPressurePlateAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PressurePlate")
	bool IsActive = false;
};
