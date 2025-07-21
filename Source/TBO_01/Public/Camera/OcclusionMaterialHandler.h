// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OcclusionMaterialHandler.generated.h"


class AGameCamera;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TBO_01_API UOcclusionMaterialHandler : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Occlusion Material Params")
	TObjectPtr<UMaterialParameterCollection> OcclusionMPC = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Occlusion Material Params", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float AdditionalDistanceCorrection = 20.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Occlusion Material Params", Meta = (UIMin = 0.0f, UIMax = 1.0f, ClampMin = 0.0f, ClampMax = 1.0f))
	float OffsetCorrection = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Occlusion Material Params", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float BehindNearObstacleDistanceCorrection = 150.0f;

	UPROPERTY()
	UMaterialParameterCollectionInstance* OcclusionMPCInstance = nullptr;
	UPROPERTY()
	AGameCamera* GameCamera = nullptr;

	float OcclusionRadius = 0.0f;
	float CurrentTargetLength = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Occlusion Material Function")
	void SetOcclusionDistance(float TargetArmLength);
	UFUNCTION(BlueprintCallable, Category = "Occlusion Material Function")
	void SetOcclusionOffset(FVector TargetOffset);
	
public:
	UFUNCTION(BlueprintCallable, Category = "Occlusion Material Functions")
	void InitializeOcclusionMaterial(AGameCamera* ParentCamera);
		
};
