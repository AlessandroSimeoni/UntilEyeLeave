// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/EQS/Context/NearbyCreature.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UNearbyCreature::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	
	UObject* Querier = QueryInstance.Owner.Get();
	if (!Querier) return;
	
	AActor* QuerierActor = Cast<AActor>(Querier);
	if (!QuerierActor) return;

	FVector QuerierLocation = QuerierActor->GetActorLocation();
	
	TArray<AActor*> NearbyCharacters;
	TArray<FOverlapResult> OverlappingActors;
    
	
	
	FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_Pawn);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(10000);
	
	if (QuerierActor->GetWorld()->OverlapMultiByObjectType(OverlappingActors, QuerierLocation, FQuat::Identity, ObjectQueryParams, Sphere))
	{
		for (const FOverlapResult& Result : OverlappingActors) 
		{
			AActor* OverlappedActor = Result.GetActor();
			if (OverlappedActor != QuerierActor)
			{
				NearbyCharacters.Add(OverlappedActor);
				
			}
		}
	}

	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, NearbyCharacters);
}
