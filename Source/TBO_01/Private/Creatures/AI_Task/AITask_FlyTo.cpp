// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/AI_Task/AITask_FlyTo.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

UAITask_FlyTo::UAITask_FlyTo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	bIsPausable = true;
	AcceptanceRadius = 100.f;
}

UAITask_FlyTo* UAITask_FlyTo::FlyTo(UObject* WorldContextObject, AAIController* Controller, AActor* GoalActor, float AcceptanceRadius)
{
	if (!Controller || !GoalActor) return nullptr;

	UAITask_FlyTo* Task = NewAITask<UAITask_FlyTo>(*Controller, EAITaskPriority::High);
	Task->AIController = Controller;
	Task->TargetActor = GoalActor;
	Task->AcceptanceRadius = AcceptanceRadius;
	Task->bUseActorAsGoal = true;
	return Task;
}

UAITask_FlyTo* UAITask_FlyTo::FlyToLocation(UObject* WorldContextObject, AAIController* Controller, FVector GoalLocation, float AcceptanceRadius)
{

	if (!Controller)
	{
		return nullptr;
	}

	UAITask_FlyTo* Task = NewAITask<UAITask_FlyTo>(*Controller, EAITaskPriority::High);
	Task->AIController = Controller;
	Task->TargetLocation = GoalLocation;
	Task->AcceptanceRadius = AcceptanceRadius;
	Task->bUseActorAsGoal = false;
	
	return Task;
}

void UAITask_FlyTo::Activate()
{
	Super::Activate();
	UE_LOG(LogTemp,Warning,TEXT("controller check %s "), *AIController->GetName());
	if (!AIController.IsValid() || !AIController->GetPawn())
	{
		OnFail.Broadcast();
		OnNativeFail.Broadcast();
		EndTask();
		return;
	}
	ACharacter* ControlledCharacter = Cast<ACharacter>(AIController.IsValid() ? AIController->GetPawn() : nullptr);

	//set flying mode
	ControlledCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void UAITask_FlyTo::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	
	ACharacter* ControlledCharacter = Cast<ACharacter>(AIController.IsValid() ? AIController->GetPawn() : nullptr);
	if (!ControlledCharacter)
	{
		OnFail.Broadcast();
		OnNativeFail.Broadcast();
		EndTask();
		return;
	}

	
	const FVector CurrentLocation = ControlledCharacter->GetActorLocation();
	const FVector Destination = bUseActorAsGoal && TargetActor.IsValid() ? TargetActor->GetActorLocation() : TargetLocation;
	const float Distance = FVector::Dist(CurrentLocation, Destination);

	if (Distance <= AcceptanceRadius)
	{
	
		//ControlledCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		ControlledCharacter->GetCharacterMovement()->StopMovementImmediately();
		OnSuccess.Broadcast();
		OnNativeSuccess.Broadcast();
		EndTask();
		return;
	}

	
	const FVector Direction = (Destination - CurrentLocation).GetSafeNormal();
	UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement();
	if (MoveComp)
	{
		//MoveComp->Velocity = Direction * MoveComp->MaxFlySpeed;
		ControlledCharacter->AddMovementInput(Direction, 1.0f);
	}

	
	//FRotator NewRotation = Direction.Rotation();
	//ControlledCharacter->SetActorRotation(NewRotation);
}

void UAITask_FlyTo::UpdateGoal(FVector NewLocation)
{
	TargetLocation = NewLocation;
	bUseActorAsGoal = false;
}

void UAITask_FlyTo::UpdateGoal(AActor* NewTargetActor)
{
	TargetActor = NewTargetActor;
	bUseActorAsGoal = true;
}

void UAITask_FlyTo::OnDestroy(bool bInOwnerFinished)
{
	if (AIController.IsValid())
	{
		if (ACharacter* Character = Cast<ACharacter>(AIController->GetPawn()))
		{
			Character->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		}
	}

	OnSuccess.Clear();       
	OnFail.Clear();
	OnNativeSuccess.Clear(); 
	OnNativeFail.Clear();

	Super::OnDestroy(bInOwnerFinished);
}