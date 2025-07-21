// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible/DestructibleParts.h"

#include "Creatures/CreatureEnumHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"

UDestructibleParts::UDestructibleParts()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDestructibleParts::BeginPlay()
{
	Super::BeginPlay();
}

bool UDestructibleParts::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity)
{
	if (ResistanceLevel > CharacterLevel)
		return false;

	AActor* OwnerActor = GetOwner();

	if (IsValid(OwnerActor))
	{
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			OwnerActor,      
			AttackInstigator,      
			DamageValue,          
			OwnerActor->GetActorLocation(), 
			AttackInstigator->GetActorLocation(),
			UCreatureEnumHelper::EnumToFName(EDamageAction::DestructionDamage)
		);
	}
	StartDestruction();
	return true;
}

void UDestructibleParts::RegisterComponent(USceneComponent* ComponentToHide)
{
	if (ComponentToHide && !RegisteredComponents.Contains(ComponentToHide))
	{
		RegisteredComponents.Add(ComponentToHide);
	}
}

void UDestructibleParts::StartDestruction()
{
	EffectData.SpawnEffect(GetWorld(),GetOwner()->GetActorLocation());
	CompleteDestruction();
}

void UDestructibleParts::CompleteDestruction()
{
	
		for (USceneComponent* Component : RegisteredComponents)
		{
			if (Component)
			{
				Component->SetVisibility(false, true);
				UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Component);
				if (PrimitiveComp)
				{
					PrimitiveComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
				TArray<USceneComponent*> ChildComponents;
				Component->GetChildrenComponents(true, ChildComponents);
				for (USceneComponent* Child : ChildComponents)
				{
					if (Child)
					{
						UPrimitiveComponent* ChildPrimitive = Cast<UPrimitiveComponent>(Child);
						if (ChildPrimitive)
						{
							ChildPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}
				}
			}
		}

		OnDestructionComplete.Broadcast();
	
}

#if WITH_EDITOR
void UDestructibleParts::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif