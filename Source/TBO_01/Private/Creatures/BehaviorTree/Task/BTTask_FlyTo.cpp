// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/BehaviorTree/Task/BTTask_FlyTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creatures/AI_Task/AITask_FlyTo.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_FlyTo::UBTTask_FlyTo()
{
	NodeName = "FlyTo";
	bNotifyTick = false;
	bNotifyTaskFinished = true;

	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FlyTo, TargetKey), AActor::StaticClass());
	TargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FlyTo, TargetKey));
}

uint16 UBTTask_FlyTo::GetInstanceMemorySize() const
{
	return sizeof(UBTTask_FlyTo);
}




EBTNodeResult::Type UBTTask_FlyTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTFlyToMemory* MyMemory = reinterpret_cast<FBTFlyToMemory*>(NodeMemory);
	MyMemory->bObserverCanFinishTask = false;

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	UObject* KeyValue = BB->GetValueAsObject(TargetKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(KeyValue);

	UAITask_FlyTo* Task = nullptr;
	if (TargetActor)
	{
		//the blackboard key is a actor
		if (bTrackMovingGoal)
		{
			//creat fly to actor task
			Task = UAITask_FlyTo::FlyTo(this, Controller, TargetActor, AcceptanceRadius);
		}
		else
		{
			//get the position of actor
			FVector StaticLocation = TargetActor->GetActorLocation();
			MyMemory->PreviousGoalLocation = StaticLocation;
			Task = UAITask_FlyTo::FlyToLocation(this, Controller, StaticLocation, AcceptanceRadius);
		}
	}
	else
	{
		FVector Location = BB->GetValueAsVector(TargetKey.SelectedKeyName);
		MyMemory->PreviousGoalLocation = Location;
		Task = UAITask_FlyTo::FlyToLocation(this, Controller, Location, AcceptanceRadius);
	}

	if (!Task)
		return EBTNodeResult::Failed;

	MyMemory->Task = Task;
	MyMemory->OnSuccessHandle = Task->OnNativeSuccess.AddUObject(this, &UBTTask_FlyTo::HandleTaskSuccess, &OwnerComp);
	MyMemory->OnFailHandle = Task->OnNativeFail.AddUObject(this, &UBTTask_FlyTo::HandleTaskFail, &OwnerComp);
	MyMemory->bObserverCanFinishTask = true;

	Task->ReadyForActivation();
	
	if (bObserveBlackboardValue)
	{
		if (MyMemory->BBObserverDelegateHandle.IsValid())
		{
			BB->UnregisterObserver(BB->GetKeyID(TargetKey.SelectedKeyName), MyMemory->BBObserverDelegateHandle);
		}

		MyMemory->BBObserverDelegateHandle = BB->RegisterObserver(
			BB->GetKeyID(TargetKey.SelectedKeyName),
			this,
			FOnBlackboardChangeNotification::CreateUObject(this, &UBTTask_FlyTo::OnBlackboardValueChange));
		
		
	
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_FlyTo::HandleTaskSuccess(UBehaviorTreeComponent* OwnerComp)
{
	if (!OwnerComp) return;

	uint8* NodeMemory = OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this));
	if (!NodeMemory) return;

	FBTFlyToMemory* MyMemory = reinterpret_cast<FBTFlyToMemory*>(NodeMemory);

	if (MyMemory->bObserverCanFinishTask)
	{
		MyMemory->bObserverCanFinishTask = false;
		OwnerComp->OnTaskFinished(this, EBTNodeResult::Succeeded);
	}
}

void UBTTask_FlyTo::HandleTaskFail(UBehaviorTreeComponent* OwnerComp)
{
	if (!OwnerComp) return;

	uint8* NodeMemory = OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this));
	if (!NodeMemory) return;

	FBTFlyToMemory* MyMemory = reinterpret_cast<FBTFlyToMemory*>(NodeMemory);

	if (MyMemory->bObserverCanFinishTask)
	{
		MyMemory->bObserverCanFinishTask = false;
		OwnerComp->OnTaskFinished(this, EBTNodeResult::Failed);
	}
}

EBTNodeResult::Type UBTTask_FlyTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTFlyToMemory* MyMemory = reinterpret_cast<FBTFlyToMemory*>(NodeMemory);

	if (MyMemory->Task.IsValid())
	{
		MyMemory->Task->OnNativeSuccess.Remove(MyMemory->OnSuccessHandle);
		MyMemory->Task->OnNativeFail.Remove(MyMemory->OnFailHandle);
		MyMemory->Task->EndTask();
		MyMemory->Task = nullptr;
	}

	MyMemory->bObserverCanFinishTask = false;

	if (bObserveBlackboardValue)
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (ensure(BB) && MyMemory->BBObserverDelegateHandle.IsValid())
		{
			BB->UnregisterObserver(BB->GetKeyID(TargetKey.SelectedKeyName), MyMemory->BBObserverDelegateHandle);
		}
		MyMemory->BBObserverDelegateHandle.Reset();

		
	}

	return EBTNodeResult::Aborted;
}

void UBTTask_FlyTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FBTFlyToMemory* MyMemory = reinterpret_cast<FBTFlyToMemory*>(NodeMemory);

	if (MyMemory->Task.IsValid())
	{
		MyMemory->Task->OnNativeSuccess.Remove(MyMemory->OnSuccessHandle);
		MyMemory->Task->OnNativeFail.Remove(MyMemory->OnFailHandle);
		MyMemory->Task->EndTask();
		MyMemory->Task = nullptr;
	}

	if (bObserveBlackboardValue)
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (ensure(BB) && MyMemory->BBObserverDelegateHandle.IsValid())
			BB->UnregisterObserver(BB->GetKeyID(TargetKey.SelectedKeyName), MyMemory->BBObserverDelegateHandle);

		MyMemory->BBObserverDelegateHandle.Reset();
	}

	MyMemory->bObserverCanFinishTask = false;
}

EBlackboardNotificationResult UBTTask_FlyTo::OnBlackboardValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	//check behavior tree is valid
	UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());
	if (!BehaviorComp)
		return EBlackboardNotificationResult::RemoveObserver;

	uint8* NodeMemory = BehaviorComp->GetNodeMemory(this, BehaviorComp->FindInstanceContainingNode(this));
	FBTFlyToMemory* MyMemory = reinterpret_cast<FBTFlyToMemory*>(NodeMemory);

	//Check if blackboard observer in memory is valid
	if (!MyMemory || !MyMemory->bObserverCanFinishTask)
		return EBlackboardNotificationResult::RemoveObserver;

	//check if task is still active
	const EBTTaskStatus::Type TaskStatus = BehaviorComp->GetTaskStatus(this);
	if (TaskStatus != EBTTaskStatus::Active)
	{
		MyMemory->BBObserverDelegateHandle.Reset();
		return EBlackboardNotificationResult::RemoveObserver;
	}

	//check if current task is valid
	if (MyMemory->Task.IsValid())
	{
		UObject* KeyValue = Blackboard.GetValueAsObject(TargetKey.SelectedKeyName);
		AActor* NewTargetActor = Cast<AActor>(KeyValue);

		if (NewTargetActor)
		{
			MyMemory->Task->UpdateGoal(NewTargetActor);
		}
		else
		{
			const FVector NewGoal = Blackboard.GetValueAsVector(TargetKey.SelectedKeyName);
			MyMemory->Task->UpdateGoal(NewGoal);
		}
	}

	return EBlackboardNotificationResult::ContinueObserving;
}
/*
	if (bUpdate)
	{
		MyMemory->bObserverCanFinishTask = false;

		if (MyMemory->Task.IsValid())
		{
			MyMemory->Task->EndTask();
			MyMemory->Task = nullptr;
		}

		if (bRestartOnBlackboardChange)
		{
			//Re execute with new parameter
			const EBTNodeResult::Type Result = ExecuteTask(*BehaviorComp, NodeMemory);
			
			if (Result != EBTNodeResult::InProgress)
			{
				FinishLatentTask(*BehaviorComp, Result);
			}
		}
		else
		{
			FinishLatentTask(*BehaviorComp, EBTNodeResult::Failed);
		}
	}

	return EBlackboardNotificationResult::ContinueObserving;
	*/

