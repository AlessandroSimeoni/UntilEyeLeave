// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/GAS/TaskFlyUp.h"

#include "AIController.h"
#include "Creatures/AI_Task/AITask_FlyTo.h"
#include "GameFramework/Character.h"

UTaskFlyUp* UTaskFlyUp::FlyUp(UGameplayAbility* OwningAbility, FName TaskInstanceName,  float InUpDistanceFromFloor)
{
	UTaskFlyUp* MyTask = NewAbilityTask<UTaskFlyUp>(OwningAbility, TaskInstanceName);
	if (MyTask)
	{
		MyTask->OwningAbilityTargetActor = OwningAbility->GetAvatarActorFromActorInfo();
		MyTask->UpDistanceFromFloor = InUpDistanceFromFloor;
		MyTask->ReadyForActivation();
	}
	return MyTask;
}

void UTaskFlyUp::Activate()
{
	Super::Activate();
	if (!OwningAbilityTargetActor.IsValid())
	{
		OnTaskFlyUpFailed.Broadcast();
		EndTask();
	}
	ACharacter* Character = Cast<ACharacter>(OwningAbilityTargetActor);

	AAIController* Controller = Cast<AAIController>(Character->GetController());
	

	FVector TargetPos = TargetUPPosition();
	
	if (TargetPos.Equals(FVector(0, 0, 0)))
	{
		EndTask();
	}
	
	UAITask_FlyTo* FlyTask = UAITask_FlyTo::FlyToLocation(this, Controller, TargetPos, 100);

	FlyTask->OnSuccess.AddDynamic(this, &UTaskFlyUp::OnFlySuccess);
	FlyTask->OnFail.AddDynamic(this, &UTaskFlyUp::OnFlyFail);

	FlyTask->ReadyForActivation();
	
}

void UTaskFlyUp::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

FVector UTaskFlyUp::TargetUPPosition()
{
	FHitResult HitResult;
	FVector Start = OwningAbilityTargetActor->GetActorLocation(); 
	FVector End = Start - FVector(0.f, 0.f, 1000.f);

	FCollisionQueryParams TraceParams(FName(TEXT("FloorTrace")), false, OwningAbilityTargetActor.Get());

	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility, 
		TraceParams
	);
	if (bHit)
	{
	
		return HitResult.ImpactPoint + FVector(0.f, 0.f, UpDistanceFromFloor);
	}
	
	OnTaskFlyUpFailed.Broadcast();
	return FVector(0, 0, 0);
}

void UTaskFlyUp::OnFlySuccess()
{
	OnTaskFlyUpFinished.Broadcast();
	EndTask();
}

void UTaskFlyUp::OnFlyFail()
{
	OnTaskFlyUpFailed.Broadcast();
	EndTask();
}