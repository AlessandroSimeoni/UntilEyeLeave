// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_BlendShape.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UAnimNotifyState_BlendShape : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend Shape")
	FName MorphTargetName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend Shape")
	float StartValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend Shape")
	float EndValue = 1.f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:

	struct FBlendShapeInfo
	{
		float Duration = 0.f;
		float StartTime = 0.f;
	};

	TMap<uint32, FBlendShapeInfo> InstanceMap;
};