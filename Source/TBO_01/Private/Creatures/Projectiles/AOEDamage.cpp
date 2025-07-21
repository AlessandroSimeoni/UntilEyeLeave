// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/AOEDamage.h"

#include "Interface/HittableActor.h"


AAOEDamage::AAOEDamage()
{

}


void AAOEDamage::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAOEDamage::Explode(float DestructionLevel, float Damage, float Size, float KnockabackIntensity)
{	
	TArray<FOverlapResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Size);
	FVector HitBoxLocation = GetActorLocation();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;
	for (auto Channel : CollisionChannels)
	{
		ObjectQueryParams.AddObjectTypesToQuery(Channel);
	}

	
	bool Hit = GetWorld()->OverlapMultiByObjectType(
   HitResults,
   HitBoxLocation,
   FQuat::Identity,
   ObjectQueryParams,
   Sphere,
   QueryParams
);

	ExplosionFX.SpawnEffect(GetWorld(),GetActorLocation(),Size*FXExplosionRadiusMultiplier,NiagaraRadiusVariableName);

	if (DebugHitBox)
		DrawDebugSphere(GetWorld(), HitBoxLocation, Size, 12, FColor::Red, false, 2.0f);

	if (Hit)
	{
		for (const FOverlapResult& Result : HitResults)
		{
			AActor* HitActor = Result.GetActor();
			UPrimitiveComponent* HitComponent = Result.GetComponent();
			if (IsValid(HitActor) && IsValid(HitComponent))
			{
				if (HitComponent->Implements<UHittableActor>())
				{
					HittableComponents.Add(HitComponent);
				}
				if (HitActor->Implements<UHittableActor>() && !HittableActors.Contains(HitActor))
				{
					HittableActors.Add(HitActor);
				}
			}
		}
		for (AActor* Actor : HittableActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attore Hittable: %s"), *Actor->GetName());
		}

		for (UPrimitiveComponent* Comp : HittableComponents)
		{
			UE_LOG(LogTemp, Warning, TEXT("Componente Hittable: %s (Owner: %s)"),
				*Comp->GetName(),
				*Comp->GetOwner()->GetName());
		}
		for (UPrimitiveComponent* HittableComponent : HittableComponents)
		{
			TScriptInterface<IHittableActor> HittableInstance = HittableComponent;
			HittableInstance->Execute_Hit(HittableComponent, DestructionLevel, Damage,
											  this,KnockabackIntensity);
		}
		for (AActor* HittableActor : HittableActors)
		{
			TScriptInterface<IHittableActor> HittableInstance = HittableActor;
			HittableInstance->Execute_Hit(HittableActor, DestructionLevel, Damage,
											  this, KnockabackIntensity);
		}
	}
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(AOEDurationTimerHandle, this, &ThisClass::EndAOE, 0.5f, false);
	
}

void AAOEDamage::EndAOE()
{
	Destroy();
}


