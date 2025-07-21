// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShootSequence.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMuzzleRandomAngle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence")
	int32 MuzzleId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence")
	float RandomAngle;

	FMuzzleRandomAngle()
		: MuzzleId(0)
		, RandomAngle(0.0f)
	{}
};
USTRUCT(BlueprintType)
struct FSequence
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Sequence")
	TArray<int32> MuzzleIds;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Sequence")
	TArray<FMuzzleRandomAngle> RandomAngle;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Sequence")
	float Delay;
	
	
	FSequence()
	: MuzzleIds()
	, RandomAngle()
	, Delay(0.0f)
	{}
};

UCLASS(BlueprintType)
class TBO_01_API UShootSequence : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Sequence")
	TArray<FSequence> ShootSequenceData;
};
