// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TaskFlyUp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskFlyUpFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskFlyUpFailed);
UCLASS()
class TBO_01_API UTaskFlyUp : public UAbilityTask
{
	GENERATED_BODY()

	public:
	UPROPERTY(BlueprintAssignable)
	FOnTaskFlyUpFinished OnTaskFlyUpFinished;
	UPROPERTY(BlueprintAssignable)
	FOnTaskFlyUpFailed OnTaskFlyUpFailed;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Fly Up", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UTaskFlyUp* FlyUp(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InUpDistanceFromFloor);
	virtual void Activate() override;
	
protected:
	virtual void OnDestroy(bool AbilityEnded) override;
	
private:

	UFUNCTION()
	void OnFlySuccess();
	UFUNCTION()
	void OnFlyFail();
	
	FVector TargetUPPosition();

	TWeakObjectPtr<AActor> OwningAbilityTargetActor;
	float UpDistanceFromFloor;
};


