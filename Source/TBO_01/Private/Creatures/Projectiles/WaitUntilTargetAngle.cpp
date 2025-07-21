// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/WaitUntilTargetAngle.h"

UWaitUntilTargetAngle* UWaitUntilTargetAngle::WaitUntilTargetAngle(UGameplayAbility* OwningAbility, AActor* TargetActor,
	float AngleDegrees, float CheckInterval)
{
	UWaitUntilTargetAngle* Task = NewAbilityTask<UWaitUntilTargetAngle>(OwningAbility);
	Task->MonitoredTarget = TargetActor;
	Task->AllowedAngle = AngleDegrees;
	Task->Interval = CheckInterval;
	return Task;
}

void UWaitUntilTargetAngle::Activate()
{
	Super::Activate();
	if (!Ability || !MonitoredTarget)
	{
		EndTask();
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UWaitUntilTargetAngle::CheckAngle, Interval, true);
	
}

void UWaitUntilTargetAngle::CheckAngle()
{
	AActor* OwnerActor = Ability->GetAvatarActorFromActorInfo();
	if (!OwnerActor || !MonitoredTarget)
	{
		OnFailed.Broadcast();
		EndTask();
		return;
	}

	FVector ToTarget = (MonitoredTarget->GetActorLocation() - OwnerActor->GetActorLocation()).GetSafeNormal();
	FVector Forward = OwnerActor->GetActorForwardVector();

	float DotProduct = FVector::DotProduct(Forward, ToTarget);
	float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

	if (AngleDegrees <= AllowedAngle)
	{
		OnTargetInAngle.Broadcast();
		EndTask();
	}
}

void UWaitUntilTargetAngle::OnDestroy(bool AbilityEnded)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::OnDestroy(AbilityEnded);
}
