// Fill out your copyright notice in the Description page of Project Settings.


#include "Eyes/EyesHandlerActorComponent.h"
#include "AbilitySystemInterface.h"
#include "Lifesystem/LifeSystemComponent.h"
#include "Utility/OptimizationSubsystem.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_ClosedEyes, "Status.ClosedEyes")

// Sets default values for this component's properties
UEyesHandlerActorComponent::UEyesHandlerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UEyesHandlerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bIsTransitioning || !EyelidMesh) return;

	
	CurrentEyesTransitionAlpha = FMath::FInterpTo(CurrentEyesTransitionAlpha, TargetEyesTransitionAlpha, DeltaTime, 1.0f / DynamicTransitionTime);

	EyelidMesh->SetMorphTarget(MeshMorphName, CurrentEyesTransitionAlpha);
	
	if (FMath::IsNearlyEqual(CurrentEyesTransitionAlpha, TargetEyesTransitionAlpha, 0.01f))
	{
		CurrentEyesTransitionAlpha = TargetEyesTransitionAlpha;
		bIsTransitioning = false;
	}
}
void UEyesHandlerActorComponent::SetUpEyes(TArray<UEyeSceneComponent*> EyeSceneComponents,float EyesHitPoint,float EyesLevel,float TransitionTime, USkeletalMeshComponent* SkeletalMeshComponent)
{
	
	Eyes = EyeSceneComponents;
	
	if (Eyes.IsEmpty())
	{
		return;
	}

	EyelidMesh = SkeletalMeshComponent;
	
	for (TObjectPtr<UEyeSceneComponent> Eye : Eyes)
	{
		if (!IsValid(Eye))
		{
			continue;
		}
		Eye->OnEyeHit.AddDynamic(this, &UEyesHandlerActorComponent::HandleEyeHit);
		Eye->OnDeath.AddDynamic(this, &UEyesHandlerActorComponent::HandleEyeDeath);
		if (!UseEyesComponentValue)
		{
			Eye->EyeLevel = EyesLevel;
			Eye->GetLifeComponent()->MaxHealth = EyesHitPoint;
			Eye->GetLifeComponent()->ResetCurrentHealth();
			EyesTransitionTime = TransitionTime;
		}

		
	}
	SetComponentTickEnabled(false);
	if (UOptimizationSubsystem* Opt =  UOptimizationSubsystem::Get(GetWorld()))
	{
			Opt->AddEyesHandlerActorComponent(this);
			
	}
	SetOptimizationTick(false);
	IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(GetOwner());
	CurrentEyesTransitionAlpha = EyelidMesh->GetMorphTarget(MeshMorphName);
	SetEyesState(EyesLidOpen,true);
	if (AbilityInterface)
	{
		ASC = AbilityInterface->GetAbilitySystemComponent();
	}
	
}

void UEyesHandlerActorComponent::SetEyesState(bool IsOpen, bool Instant)
{
	EyesLidOpen = IsOpen;
	TargetEyesTransitionAlpha = IsOpen ? 0.0f : 1.0f;
	if (IsValid(ASC))
	{
		if (IsOpen)
		{
			ASC->RemoveLooseGameplayTag(TAG_ClosedEyes);
		}
		else
		{
			ASC->AddLooseGameplayTag(TAG_ClosedEyes);
		}
	}
	
	if (Instant)
	{
		CurrentEyesTransitionAlpha = TargetEyesTransitionAlpha;
	}
	
	float RemainingDistance = FMath::Abs(CurrentEyesTransitionAlpha - TargetEyesTransitionAlpha);
	DynamicTransitionTime = EyesTransitionTime * RemainingDistance;
	bIsTransitioning = true;
	

	for (TObjectPtr<UEyeSceneComponent> Eye : Eyes)
	{
		if (IsValid(Eye))
		{
			Eye->ChangeAyesHurtbox(IsOpen);
			Eye->EyeMesh->SetComponentTickEnabled(false);
		}
	}
}

void UEyesHandlerActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UOptimizationSubsystem* Opt =  UOptimizationSubsystem::Get(GetWorld()))
	{
		Opt->RemoveEyesHandlerActorComponent(this);
	}
	Super::EndPlay(EndPlayReason);
}

void UEyesHandlerActorComponent::HandleEyeHit(UEyeSceneComponent* Eye, float Damage, const AActor* ActorInstigator,
                                              float Force)
{
	if (OnEyesHit.IsBound())
		OnEyesHit.Broadcast(Eye, Damage, ActorInstigator, Force);
}

void UEyesHandlerActorComponent::HandleEyeDeath(UEyeSceneComponent* Eye,const AActor* LastInstigator)
{
	

	if (CheckAllEyesDeath())
	{
		if (OnAllEyesDeath.IsBound())
			OnAllEyesDeath.Broadcast(Eye,LastInstigator);
	}
	else
	{
		if (OnEyeDeath.IsBound())
			OnEyeDeath.Broadcast(Eye,LastInstigator);
	}
}
bool UEyesHandlerActorComponent::CheckAllEyesDeath()
{
	for (TObjectPtr<UEyeSceneComponent> Eye : Eyes)
	{
		if (!Eye->IsEyeDestroyed())
			return false;
	}
	return true;
}

float UEyesHandlerActorComponent::LivingEyesRemaining()
{
	
	float TotalEyes = Eyes.Num();

	if (TotalEyes == 0)
		return 0.0f;
	
	float LivingEyes = TotalEyes;
	
	for (TObjectPtr<UEyeSceneComponent> Eye : Eyes)
	{
		if (!Eye->IsEyeDestroyed())
			LivingEyes --;
	}
	return LivingEyes/TotalEyes;
}




void UEyesHandlerActorComponent::SetEyesTargetPlayer(AActor* Target)
{
	for (TObjectPtr<UEyeSceneComponent> Eye : Eyes)
	{
		if (IsValid(Eye))
		{
			Eye->SetEyeTarget(Target);
		}
	}
}
void UEyesHandlerActorComponent::RemoveEyesTargetPlayer(AActor* Target)
{
	if (Eyes.IsEmpty())
		return;
	for (TObjectPtr<UEyeSceneComponent> Eye : Eyes)
	{
		if (IsValid(Eye))
		{
			Eye->RemoveEyeTarget(Target);
		}
	}
}
void UEyesHandlerActorComponent::RestoreEyes()
{
	if (Eyes.IsEmpty())
		return;
	for (TObjectPtr<UEyeSceneComponent> Eye : Eyes)
	{
		if (IsValid(Eye))
		{
			Eye->RestoreEyeComponent();
		}
	}
}

void UEyesHandlerActorComponent::SetOptimizationTick(bool bIsInRange)
{
	if (bOptimizeTick == bIsInRange)
	{
		return;
	}
	
	SetComponentTickEnabled(bIsInRange);
	for (TObjectPtr<UEyeSceneComponent> Eye : Eyes)
	{
		Eye->SetComponentTickEnabled(bIsInRange);
		Eye->EyeMesh->SetComponentTickEnabled(bIsInRange);
		//UE_LOG(LogTemp,Warning,TEXT("UEyesHandlerActorComponent::SetOptimizationTick"));
	}
	
	bOptimizeTick = bIsInRange;
}
