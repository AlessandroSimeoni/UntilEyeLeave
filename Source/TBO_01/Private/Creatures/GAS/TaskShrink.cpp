// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/GAS/TaskShrink.h"

UTaskShrink* UTaskShrink::ShrinkMesh(UObject* TaskOwner, USkeletalMeshComponent* Mesh, float Duration, float Scale)
{
	if (!TaskOwner || !Mesh)
		return nullptr;

	UTaskShrink* MyTask = NewTask<UTaskShrink>(TaskOwner);
	MyTask->TargetMesh = Mesh;
	MyTask->TotalDuration = Duration;
	MyTask->TargetScale = Scale;
	return MyTask;
	
	
}

void UTaskShrink::Activate()
{
	Super::Activate();
	if (!TargetMesh)
	{
		EndTask();
		return;
	}
	Elapsed = 0.f;
	bIsDone = false;
	OriginalScale = TargetMesh->GetRelativeScale3D();
	bTickingTask = true;
	
	
}

void UTaskShrink::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (bIsDone || !TargetMesh)
	{
		EndTask();
		return;
	}

	Elapsed += DeltaTime;

	float Alpha = FMath::Clamp(Elapsed / TotalDuration, 0.f, 1.f);
	float CurrentScaleFactor = FMath::Lerp(1.f, TargetScale, Alpha);
	FVector NewScale = OriginalScale * CurrentScaleFactor;
	TargetMesh->SetRelativeScale3D(NewScale);
	
	if (Elapsed >= TotalDuration)
	{
		OnFinished.Broadcast();
		bIsDone = true;
		EndTask();
		return;
	}
	
}

void UTaskShrink::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	if (TargetMesh)
	{
		FVector NewScale = OriginalScale * TargetScale;
		TargetMesh->SetRelativeScale3D(NewScale);
	}
	
}
