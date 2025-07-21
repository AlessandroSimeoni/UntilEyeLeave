// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/ScriptedController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Creatures/Creature.h"
#include "Creatures/Patrol/ScriptedPatrolPath.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values
AScriptedController::AScriptedController(): PawnCreature(nullptr), BehaviorTreeAsset(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AScriptedController::BeginPlay()
{
	Super::BeginPlay();
}

void AScriptedController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
		SetStop(true);
	}

	PawnCreature = Cast<ACreature>(InPawn);
	if (PawnCreature && PawnCreature->GetClass()->ImplementsInterface(UPatrolInterface::StaticClass()))
	{
		UPatrolSplineComponent* SplineComp = IPatrolInterface::Execute_GetPatrolSplineComponent(PawnCreature);
		if (SplineComp)
		{
			SplineComp->OnScriptedAction.AddUObject(this, &AScriptedController::DoScriptedAction);

			if (IsValid(GetBlackboardComponent()))
			{
				GetBlackboardComponent()->SetValueAsBool(KeyNameIsPatrolPathAssigned, true);
			}
		}
	}
}
void AScriptedController::OnUnPossess()
{
	Super::OnUnPossess();

	if (PawnCreature && PawnCreature->GetClass()->ImplementsInterface(UPatrolInterface::StaticClass()))
	{
		UPatrolSplineComponent* SplineComp = IPatrolInterface::Execute_GetPatrolSplineComponent(PawnCreature);
		if (SplineComp)
		{
			SplineComp->OnScriptedAction.RemoveAll(this);
		}
	}

	PawnCreature = nullptr;
}

void AScriptedController::SetStop(bool Stop)
{
	if (IsValid(GetBlackboardComponent()))
	{
		GetBlackboardComponent()->SetValueAsBool(KeyNameStop, Stop);
	}
	if (bPathCompleted)
	{
		OnScriptedPatrolCompleted.Broadcast();
		if (bResetPathOnEnd)
		{
			ResetPath();
		}
	}
}



void AScriptedController::DoScriptedAction(const FScriptedAction& ScriptedAction)
{
	if (!PawnCreature)
	{
		return;
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ActivateAbility"), ScriptedAction);
	UE_LOG(LogTemp, Warning, TEXT("Activate scripted Ability"));
	SetStop(true);
	if (ScriptedAction.DelayBefore > 0)
	{
		PawnCreature->GetWorld()->GetTimerManager().SetTimer(CurrentTimer, TimerDelegate, ScriptedAction.DelayBefore,
		                                                     false);
	}
	else
	{
	
		ActivateAbility(ScriptedAction);
	}
}

void AScriptedController::ActivateAbility(const FScriptedAction& ScriptedAction)
{
	if (!PawnCreature || !ScriptedAction.Ability)
	{
		return;
	}
	PawnCreature->GetMovementComponent()->StopMovementImmediately();
	
	CurrentScriptedAction = ScriptedAction;

	if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(PawnCreature))
	{
		if (UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
		{
			if (ScriptedAction.Event)
			{
				const FGameplayTag EventTag = ScriptedAction.EventTag;

				FGameplayEventData EventData;
				EventData.Instigator = PawnCreature;
				EventData.EventTag = EventTag;
				EventData.Target = ScriptedAction.TargetActor;

				ASC->HandleGameplayEvent(EventTag, &EventData);

				if (ScriptedAction.DelayAfter > 0)
				{
					FTimerDelegate TimerDelegate;
					TimerDelegate.BindUFunction(this, FName("SetStop"), false);
					PawnCreature->GetWorld()->GetTimerManager().SetTimer(CurrentTimer, TimerDelegate, ScriptedAction.DelayAfter, false);
				}
				else
				{
					SetStop(false);
				}
			}
			else
			{
			
				FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(ScriptedAction.Ability);
				if (Spec)
				{
					ActiveAbilityHandle = Spec->Handle;
					
					ASC->OnAbilityEnded.RemoveAll(this); 
					ASC->OnAbilityEnded.AddUObject(this, &AScriptedController::OnAbilityEndedCallback);
					
					bool bSuccess = ASC->TryActivateAbility(ActiveAbilityHandle);

					UE_LOG(LogTemp, Log, TEXT("TryActivateAbility %s: %s"),
					       *ScriptedAction.Ability->GetName(),
					       bSuccess ? TEXT("SUCCESS") : TEXT("FAILURE"));

		
					if (!bSuccess)
					{
						ASC->OnAbilityEnded.RemoveAll(this);
						SetStop(false);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Ability Spec not found for ability %s"), *ScriptedAction.Ability->GetName());
					SetStop(false);
				}
			}
		}
	}
}


void AScriptedController::OnAbilityEndedCallback(const FAbilityEndedData& Data)
{
	if (!PawnCreature || !CurrentScriptedAction.Ability)
	{
		return;
	}

	if (Data.AbilityThatEnded == nullptr || Data.AbilitySpecHandle != ActiveAbilityHandle)
	{
		return; 
	}

	UE_LOG(LogTemp, Log, TEXT("Ability %s finished. DelayAfter: %f"),
		   *Data.AbilityThatEnded->GetName(), CurrentScriptedAction.DelayAfter);

	
	if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(PawnCreature))
	{
		if (UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
		{
			ASC->OnAbilityEnded.RemoveAll(this);
		}
	}

	
	if (CurrentScriptedAction.DelayAfter > 0)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("SetStop"), false);
		PawnCreature->GetWorld()->GetTimerManager().SetTimer(CurrentTimer, TimerDelegate, CurrentScriptedAction.DelayAfter, false);
	}
	else
	{
		SetStop(false);
	}
}

void AScriptedController::ResetPath()
{
	UPatrolSplineComponent* SplineComp = IPatrolInterface::Execute_GetPatrolSplineComponent(PawnCreature);
	SplineComp->CurrentIndex = 0;

	if (!IsValid(Blackboard))
	{
		return;
	}

	Blackboard->SetValueAsBool(KeyNameIsPatrolPathAssigned, true);
	Blackboard->ClearValue(KeyNamePosition);
	bPathCompleted = false;
}

