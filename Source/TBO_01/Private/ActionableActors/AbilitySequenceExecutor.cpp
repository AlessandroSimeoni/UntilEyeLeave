#include "ActionableActors/AbilitySequenceExecutor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Creatures/DataAsset/CreatureAbilitySequence.h"

void AAbilitySequenceExecutor::RegisterAbilitySequenceWithTarget(UCreatureAbilitySequence* Sequence, AActor* OwnerActor, AActor* TargetActor)
{
	if (!Sequence || !OwnerActor) return;

	FRegisteredAbilitySequence& Entry = RegisteredSequences.FindOrAdd(OwnerActor);
	Entry.Sequence = Sequence;
	Entry.Target = TargetActor;
}

void AAbilitySequenceExecutor::StartAbilitySequence(UCreatureAbilitySequence* Sequence, AActor* OwnerActor)
{
	if (!Sequence || !OwnerActor) return;

	FAbilitySequenceState& State = ActiveSequences.FindOrAdd(OwnerActor);
	State.Sequence = Sequence;
	State.Index = 0;

	ScheduleNextAbility(OwnerActor);
}

void AAbilitySequenceExecutor::StopAbilitySequence(AActor* OwnerActor)
{
	if (FAbilitySequenceState* State = ActiveSequences.Find(OwnerActor))
	{
		if (OwnerActor && OwnerActor->GetWorld())
		{
			OwnerActor->GetWorld()->GetTimerManager().ClearTimer(State->TimerHandle);
		}
		ActiveSequences.Remove(OwnerActor);
	}
}

void AAbilitySequenceExecutor::StartAllAbilitySequences()
{
	for (const auto& Pair : RegisteredSequences)
	{
		if (Pair.Key.IsValid() && Pair.Value.Sequence)
		{
			StartAbilitySequence(Pair.Value.Sequence, Pair.Key.Get());
		}
	}
}

void AAbilitySequenceExecutor::StopAllAbilitySequences()
{
	for (auto& Pair : ActiveSequences)
	{
		if (AActor* OwnerActor = Pair.Key.Get())
		{
			if (OwnerActor->GetWorld())
			{
				OwnerActor->GetWorld()->GetTimerManager().ClearTimer(Pair.Value.TimerHandle);
			}
		}
	}
	ActiveSequences.Empty();
}

void AAbilitySequenceExecutor::ScheduleNextAbility(AActor* OwnerActor)
{
	
	if (!OwnerActor) return;

	FAbilitySequenceState* State = ActiveSequences.Find(OwnerActor);
	if (!State || !State->Sequence || State->Index >= State->Sequence->AbilityDelays.Num()) return;

	const FAbilityDelay& Current = State->Sequence->AbilityDelays[State->Index];
	

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ExecuteAbility"), OwnerActor);

	OwnerActor->GetWorld()->GetTimerManager().SetTimer(State->TimerHandle, TimerDelegate, Current.Delay, false);
}

void AAbilitySequenceExecutor::TriggerNextEventAbility(AActor* OwnerActor)
{
	if (!OwnerActor) return;

	FAbilitySequenceState* State = ActiveSequences.Find(OwnerActor);
	if (!State || !State->Sequence || State->Index >= State->Sequence->AbilityDelays.Num()) return;

	const FAbilityDelay& Current = State->Sequence->AbilityDelays[State->Index];
	if (!Current.Event) return;

	ActivateAbility(OwnerActor, Current);

	State->Index++;
	ScheduleNextAbility(OwnerActor);
}

void AAbilitySequenceExecutor::ExecuteAbility(AActor* OwnerActor)
{
	if (!OwnerActor) return;

	FAbilitySequenceState* State = ActiveSequences.Find(OwnerActor);
	if (!State || !State->Sequence || State->Index >= State->Sequence->AbilityDelays.Num()) return;

	const FAbilityDelay& Current = State->Sequence->AbilityDelays[State->Index];

	ActivateAbility(OwnerActor, Current);

	State->Index++;
	ScheduleNextAbility(OwnerActor);
}

void AAbilitySequenceExecutor::ActivateAbility(AActor* OwnerActor, const FAbilityDelay& AbilityData)
{
	
	if (!OwnerActor || !AbilityData.Ability) return;

	if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(OwnerActor))
	{
		if (UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
		{
			if (AbilityData.Event)
			{
				const FGameplayTag EventTag = AbilityData.EventTag;

				FGameplayEventData EventData;
				EventData.Instigator = OwnerActor;
				EventData.EventTag = EventTag;
				EventData.Target = nullptr;

				if (const FRegisteredAbilitySequence* Entry = RegisteredSequences.Find(OwnerActor))
				{
					if (Entry->Target.IsValid())
					{
						EventData.Target = Entry->Target.Get();
					}
				}

				ASC->HandleGameplayEvent(EventTag, &EventData);
				UE_LOG(LogTemp, Log, TEXT("Ability Sequence Activated"));
			}
			else
			{
				ASC->TryActivateAbilityByClass(AbilityData.Ability);
			}
		}
	}
}
