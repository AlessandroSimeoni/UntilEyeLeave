// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/GAS/TaskVibrateMesh.h"

#include "GameFramework/Actor.h"

UTaskVibrateMesh* UTaskVibrateMesh::VibrateMesh(UObject* TaskOwner, USkeletalMeshComponent* Mesh, float Duration, float Intensity)
{
	if (!TaskOwner || !Mesh)
		return nullptr;

	UTaskVibrateMesh* MyTask = NewTask<UTaskVibrateMesh>(TaskOwner);
	MyTask->TargetMesh = Mesh;
	MyTask->TotalDuration = Duration;
	MyTask->VibrationIntensity = Intensity;
	return MyTask;
}

void UTaskVibrateMesh::Activate()
{
	Super::Activate();

	if (!TargetMesh)
	{
		EndTask();
		return;
	}

	OriginalLocation = TargetMesh->GetRelativeLocation();
	Elapsed = 0.f;
	bIsDone = false;
	bTickingTask = true;
	
}

void UTaskVibrateMesh::TickTask(float DeltaTime)
{
	if (bIsDone || !TargetMesh)
	{
		EndTask();
		return;
	}

	Elapsed += DeltaTime;

	if (Elapsed >= TotalDuration)
	{
		TargetMesh->SetRelativeLocation(OriginalLocation);
		OnFinished.Broadcast();
		bIsDone = true;
		EndTask();
		return;
	}

	FVector RandomOffset = FVector(
		FMath::FRandRange(-1.f, 1.f),
		FMath::FRandRange(-1.f, 1.f),
		FMath::FRandRange(-1.f, 1.f)
	) * VibrationIntensity;

	FVector NewLoc = OriginalLocation + RandomOffset;
	TargetMesh->SetRelativeLocation(NewLoc);
}

void UTaskVibrateMesh::OnDestroy(bool bInOwnerFinished)
{
	if (TargetMesh)
	{
		TargetMesh->SetRelativeLocation(OriginalLocation);
	}

	Super::OnDestroy(bInOwnerFinished);
}