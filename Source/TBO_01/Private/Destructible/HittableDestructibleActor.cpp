// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible/HittableDestructibleActor.h"

#include "Currency/CoinContainer.h"
#include "Utility/OptimizationSubsystem.h"



bool AHittableDestructibleActor::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity)
{
	UE_LOG(LogTemp, Warning, TEXT("HittableDestructibleActor::Hit_Implementation %d"),CharacterLevel);
	
	if (ResistanceLevel > CharacterLevel)
	{
		return false; 
	}

	CoinContainer->SpawnCoin(AttackInstigator->GetActorLocation());
	Destruction();
	return true;
}

void AHittableDestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	SkeletalMeshComponent = MakeWeakObjectPtr(FindComponentByClass<USkeletalMeshComponent>());
	if (SkeletalMeshComponent.IsValid())
	{
		if (UOptimizationSubsystem* Opt =  UOptimizationSubsystem::Get(GetWorld()))
		{
			Opt->AddHittableDestructibleActor(this);
		}
	}
}

void AHittableDestructibleActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!SkeletalMeshComponent.IsValid())
		return;
	
	if (UOptimizationSubsystem* Opt =  UOptimizationSubsystem::Get(GetWorld()))
	{
		Opt->RemoveHittableDestructibleActor(this);
	}
	Super::EndPlay(EndPlayReason);
}
