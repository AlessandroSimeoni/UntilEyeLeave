// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/GAS/TaskFall.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

UTaskFall::UTaskFall(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

UTaskFall* UTaskFall::Fall(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UTaskFall* MyTask = NewAbilityTask<UTaskFall>(OwningAbility, TaskInstanceName);
	if (MyTask)
	{
		MyTask->OwningAbilityTargetActor = OwningAbility->GetAvatarActorFromActorInfo();
		MyTask->ReadyForActivation();
	}
	return MyTask;
}


void UTaskFall::Activate()
{
	Super::Activate();
	if (IsValid(OwningAbilityTargetActor))
	{
		ACharacter* Character = Cast<ACharacter>(OwningAbilityTargetActor);
		if (IsValid(Character))
		{
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			Character->MovementModeChangedDelegate.AddDynamic(this, &UTaskFall::OnMovementModeChange);
		}
		else
		{
			
			EndTask();
		}
	}
	
}

void UTaskFall::OnDestroy(bool AbilityEnded)
{
	
	if (AbilityEnded)
	{
		
	}
	Super::OnDestroy(AbilityEnded);
}

void UTaskFall::OnMovementModeChange(ACharacter* Character, EMovementMode PrevMode, uint8 PrevCustomMode)
{
	if (Character->GetCharacterMovement()->IsMovingOnGround())
	{
		UE_LOG(LogTemp, Log, TEXT("Fall Complete"));
		OnFallFinished.Broadcast();
		EndTask();
	}
}
