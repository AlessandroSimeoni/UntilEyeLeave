// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "TaskShrink.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShrinkFinished);
UCLASS()
class TBO_01_API UTaskShrink : public UGameplayTask
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "TaskOwner", DefaultToSelf = "TaskOwner"))
	static UTaskShrink* ShrinkMesh( UObject* TaskOwner,USkeletalMeshComponent* Mesh, float Duration, float Scale);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintAssignable)
	FOnShrinkFinished OnFinished;

private:
	UPROPERTY()
	USkeletalMeshComponent* TargetMesh;
	float TotalDuration;
	float Elapsed;
	float TargetScale;
	FVector OriginalScale;
	bool bIsDone;
};
