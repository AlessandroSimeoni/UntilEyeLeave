// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AITask_FlyTo.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FFlyToNativeDelegate);
UCLASS()
class TBO_01_API UAITask_FlyTo : public UAITask
{
	GENERATED_BODY()

public:
	UAITask_FlyTo(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "AI|Tasks", meta = (DefaultToSelf = "Controller", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAITask_FlyTo* FlyTo(UObject* WorldContextObject, AAIController* Controller, AActor* GoalActor, float AcceptanceRadius = 100.f);

	UFUNCTION(BlueprintCallable, Category = "AI|Tasks", meta = (DefaultToSelf = "Controller", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAITask_FlyTo* FlyToLocation(UObject* WorldContextObject, AAIController* Controller, FVector GoalLocation, float AcceptanceRadius = 100.f);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;


	void UpdateGoal(FVector NewLocation);
	
	void UpdateGoal(AActor* NewTargetActor);
	
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnFail;

	FFlyToNativeDelegate OnNativeSuccess;
	FFlyToNativeDelegate OnNativeFail;

	
	UPROPERTY()
	TWeakObjectPtr<UBehaviorTreeComponent> BoundBehaviorTree;
private:
	TWeakObjectPtr<AAIController> AIController;
	TWeakObjectPtr<AActor> TargetActor;
	FVector TargetLocation;
	float AcceptanceRadius;
	bool bUseActorAsGoal = false;
};
