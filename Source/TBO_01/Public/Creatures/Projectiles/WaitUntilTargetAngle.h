// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitUntilTargetAngle.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitTargetInAngleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitTargetInAngleFailedDelegate);
UCLASS()
class TBO_01_API UWaitUntilTargetAngle : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Wait Until Target In Angle", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UWaitUntilTargetAngle* WaitUntilTargetAngle(UGameplayAbility* OwningAbility, AActor* TargetActor, float AngleDegrees, float CheckInterval = 0.1f);

	UPROPERTY(BlueprintAssignable)
	FWaitTargetInAngleDelegate OnTargetInAngle;
	UPROPERTY(BlueprintAssignable)
	FWaitTargetInAngleFailedDelegate OnFailed;

	virtual void Activate() override;

protected:
	UPROPERTY()
	AActor* MonitoredTarget;

	UPROPERTY()
	float AllowedAngle;

	UPROPERTY()
	float Interval;

	FTimerHandle TimerHandle;

	void CheckAngle();

	virtual void OnDestroy(bool AbilityEnded) override;
	
};
