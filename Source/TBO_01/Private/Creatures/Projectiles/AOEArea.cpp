// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/AOEArea.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "NiagaraComponent.h"
#include "Character/MainCharacter.h"
#include "Components/SphereComponent.h"


AAOEArea::AAOEArea()
{
	PrimaryActorTick.bCanEverTick = true;
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	RootComponent = NiagaraComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(0.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
	SphereComponent->SetupAttachment(NiagaraComponent);
}

void AAOEArea::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnBeginOverlap);
		SphereComponent->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnEndOverlap);
	}
	
	RemoveActiveEffects(nullptr);
}

void AAOEArea::EndAOE()
{
	AOETargetRadius = 0;
	CurrentSpawnRate = 0;
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(AOEEndDurationTimerHandle, this, &ThisClass::CompleteEndAOE, AOEEndDuration, false);
}

void AAOEArea::CompleteEndAOE()
{
	CurrentSpawnRate = NiagaraInitialSpawnRate;
	AOEEnding = true;
	SphereComponent->SetActive(false);
	GetWorld()->GetTimerManager().ClearTimer(AOETickTimerHandle);
}

void AAOEArea::RemoveActiveEffects(UAbilitySystemComponent* TargetASC)
{
	if (TargetASC)
	{
		if (FActiveGameplayEffectHandle* Handle = ActiveEffectsMap.Find(TargetASC))
		{
			TargetASC->RemoveActiveGameplayEffect(*Handle);
			ActiveEffectsMap.Remove(TargetASC);
		}
	}
	else
	{
		for (const TPair<UAbilitySystemComponent*, FActiveGameplayEffectHandle>& Pair : ActiveEffectsMap)
		{
			if (Pair.Key)
			{
				Pair.Key->RemoveActiveGameplayEffect(Pair.Value);
			}
		}
		
		ActiveEffectsMap.Empty();
	}
}

void AAOEArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && !OverlappingActors.Contains(OtherActor) && OtherActor->Implements<UAbilitySystemInterface>())
	{
		OverlappingActors.Add(OtherActor);
		
		ApplyEffect(OtherActor);
	}
}
void AAOEArea::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor->Implements<UAbilitySystemInterface>())
	{
		OverlappingActors.Remove(OtherActor);
		//ApplyEffect(OtherActor);
		RemoveEffect(OtherActor);
	}
}

void AAOEArea::ApplyEffect(AActor* OtherActor)
{
	if (!OtherActor->Implements<UAbilitySystemInterface>())
		return;
	
	const TScriptInterface<IAbilitySystemInterface> AbilitySystemInstance = OtherActor;
	FGameplayEventData EventData;
	EventData.Instigator = this;
	EventData.Target = OtherActor;
	EventData.EventMagnitude = 1.0f;
	EventData.OptionalObject = nullptr;
	EventData.OptionalObject2 = nullptr;

	AbilitySystemInstance->GetAbilitySystemComponent()->HandleGameplayEvent(AOEDOTAbilityTag, &EventData);
	
}
 void AAOEArea::RemoveEffect(AActor* OtherActor)
{
	if (!OtherActor->IsA(AMainCharacter::StaticClass()))
		return;
	
	const TScriptInterface<IAbilitySystemInterface> AbilitySystemInstance = OtherActor;
	UAbilitySystemComponent* ASC = AbilitySystemInstance->GetAbilitySystemComponent();
	if (!ASC)
		return;


	FGameplayTagContainer TagsToRemove;
	TagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Status.Charged"));

	int32 NumRemoved = ASC->RemoveActiveEffectsWithGrantedTags(TagsToRemove);
	//UE_LOG(LogTemp, Log, TEXT("Removed %d effects with granted tags."), NumRemoved);
	
}


void AAOEArea::BeginPlay()
{
	Super::BeginPlay();

	NiagaraComponent->SetFloatParameter(NiagaraSpawnRateVariableName, NiagaraInitialSpawnRate);
	FTimerManager& TM = GetWorld()->GetTimerManager();
	
	if (AOEDuration >= 0)
		TM.SetTimer(AOEDurationTimerHandle, this, &ThisClass::EndAOE, AOEDuration, false);

	GetWorld()->GetTimerManager().SetTimer(AOETickTimerHandle, this, &ThisClass::ApplyEffectsToOverlappingActors, AOEInterval, true);
}

void AAOEArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AOEEnding)
	{
		CurrentSpawnRate -= SpawnRateFalloffSpeed * DeltaTime;
		if (CurrentSpawnRate <= 0.0f)
			CurrentSpawnRate = 0.0f;
		
		NiagaraComponent->SetFloatParameter(NiagaraSpawnRateVariableName, CurrentSpawnRate);

		if (CurrentSpawnRate == 0.0f)
			Destroy();
	}
	
		CurrentRadius = FMath::FInterpTo(CurrentRadius, AOETargetRadius, DeltaTime, RadiusChangeSpeed);
		NiagaraComponent->SetFloatParameter(NiagaraRadiusVariableName, CurrentRadius);
		SphereComponent->SetSphereRadius(CurrentRadius);
	
}

void AAOEArea::NotifyEffect_Implementation(UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	ActiveEffectsMap.Add(ASC, ActiveEffectHandle);
}

