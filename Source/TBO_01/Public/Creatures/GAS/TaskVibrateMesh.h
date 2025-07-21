// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "TaskVibrateMesh.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVibrationFinished);
UCLASS()
class TBO_01_API UTaskVibrateMesh : public UGameplayTask
{
	GENERATED_BODY()
	public:
        UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "TaskOwner", DefaultToSelf = "TaskOwner"))
        static UTaskVibrateMesh* VibrateMesh( UObject* TaskOwner,USkeletalMeshComponent* Mesh, float Duration, float Intensity);
    
        virtual void Activate() override;
        virtual void TickTask(float DeltaTime) override;
		virtual void OnDestroy(bool bInOwnerFinished) override;
    
        UPROPERTY(BlueprintAssignable)
        FOnVibrationFinished OnFinished;
    
    private:
		UPROPERTY()
        USkeletalMeshComponent* TargetMesh;
        FVector OriginalLocation;
        float TotalDuration;
        float VibrationIntensity;
        float Elapsed;
    
        bool bIsDone;
	
};
