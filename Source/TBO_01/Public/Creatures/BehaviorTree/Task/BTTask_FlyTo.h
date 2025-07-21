// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyTo.generated.h"

class UAITask_FlyTo;

struct FBTFlyToMemory
{
	TWeakObjectPtr<UAITask_FlyTo> Task;
	FDelegateHandle OnSuccessHandle;
	FDelegateHandle OnFailHandle;
	FDelegateHandle BBObserverDelegateHandle;
	bool bObserverCanFinishTask = false;
	FVector PreviousGoalLocation = FAISystem::InvalidLocation;
};
UCLASS()
class TBO_01_API UBTTask_FlyTo : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FlyTo();

	UPROPERTY(EditAnywhere,Category="Blackboard")
	 FBlackboardKeySelector TargetKey;
	
	
	UPROPERTY(EditAnywhere,Category="Task")
	float AcceptanceRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Task")
	bool bTrackMovingGoal = true;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bObserveBlackboardValue = false;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bRestartOnBlackboardChange = true;



protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual uint16 GetInstanceMemorySize() const override;
	
	virtual EBlackboardNotificationResult OnBlackboardValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

	UFUNCTION()
	void HandleTaskSuccess(UBehaviorTreeComponent* OwnerComp);

	UFUNCTION()
	void HandleTaskFail(UBehaviorTreeComponent* OwnerComp);
};
