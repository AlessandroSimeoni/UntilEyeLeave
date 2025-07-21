// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GAS/PlayerHitAbility.h"

#include "AbilitySystemComponent.h"
#include "Character/GAS/PlayerAttributeSet.h"
#include "FSM/StateController.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DeflectHit,"FeedBack.DeflectHit")

UPlayerHitAbility::UPlayerHitAbility()
{
	//dont refresh parameter every use
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPlayerHitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{

	if (!HasAuthority(&ActivationInfo)) return;
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;
	const AActor* Target = TriggerEventData->Target;
	const UPrimitiveComponent* HitBox = Cast<UPrimitiveComponent>(TriggerEventData->OptionalObject);
	USceneComponent* ParentComponent = HitBox->GetAttachParent();


	if (HitBox->Implements<UHittableActor>())
	{
		ParentComponent = const_cast<USceneComponent*>(Cast<USceneComponent>(HitBox));
		
	}
	
	//if hitting deflecting collider
	if (HitBox->ComponentTags.Contains(DeflectTag))
	{
		CombatState->Deflect(Target,HitBox, TriggerEventData->ContextHandle.GetOrigin());
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target))
		{
			UE_LOG(LogTemp,Warning,TEXT("target %s"), *Target->GetName())
			if (UAbilitySystemComponent* TargetASC = AbilitySystemInterface->GetAbilitySystemComponent())
			{
				TargetASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_DeflectHit),true);
				FGameplayEventData EventData;
				EventData.EventTag = TAG_DeflectHit;
				EventData.Instigator = ActorInfo->OwnerActor.Get();
				TargetASC->HandleGameplayEvent(TAG_DeflectHit, &EventData);
			}
		}
	}
	if (Target) //check if the actor implement the hittable interface
	{
		if (Target->Implements<UHittableActor>())
		{
			const IHittableActor* HittableTarget = Cast<IHittableActor>(Target);
			if (HittableTarget)
			{
				
				UE_LOG(LogTemp, Warning, TEXT("Hit Target: %s"), *Target->GetName());
				if (HittableTarget->Execute_Hit(const_cast<AActor*>(Target), AttributesSet->GetWeaponLevel(), AttributesSet->GetAttack(), MyCharacter,1.f))
				{
					CombatState->Hit(Target,HitBox, TriggerEventData->ContextHandle.GetOrigin());
				}
				else
				{
					CombatState->Deflect(Target,HitBox, TriggerEventData->ContextHandle.GetOrigin());
				}
			}
		}
		else if (ParentComponent && ParentComponent->Implements<UHittableActor>()) //check if the component implement the hittable interface
		{
			TScriptInterface<IHittableActor> HittableInstance = ParentComponent;
			UE_LOG(LogTemp, Warning, TEXT("Hit Box: %s"), *ParentComponent->GetName());
			if (HittableInstance->Execute_Hit(ParentComponent, AttributesSet->GetWeaponLevel(), AttributesSet->GetAttack(), MyCharacter,1.f))
			{
			CombatState->Hit(Target,HitBox, TriggerEventData->ContextHandle.GetOrigin());
			}
			else
			{
			CombatState->Deflect(Target,HitBox, TriggerEventData->ContextHandle.GetOrigin());
			}
			
		}
	}

	K2_ActivateAbilityFromEvent(*TriggerEventData);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPlayerHitAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	//set up reused parameter
	if (AActor* Avatar = ActorInfo->AvatarActor.Get())
	{
		MyCharacter = Cast<AMainCharacter>(Avatar);
		if (MyCharacter)
		{
			AttributesSet = MyCharacter->GetAbilitySystemComponent()->GetSet<UPlayerAttributeSet>();
			if (UStateBase* State = MyCharacter->StateControllerComponent->FindStateOfClass(CombatStateClass))
			{
				CombatState = Cast<UPlayerCombatState>(State);
				
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to cast State to CombatState."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast AvatarActor to AMainCharacter."));
		}
	}
}