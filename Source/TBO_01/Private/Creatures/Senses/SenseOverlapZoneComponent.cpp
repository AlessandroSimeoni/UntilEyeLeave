// Fill out your copyright notice in the Description page of Project Settings.

#include "Creatures/Senses/SenseOverlapZoneComponent.h"

#include "Character/MainCharacter.h"
#include "Perception/AISense_Damage.h"
#include "Creatures/CreatureEnumHelper.h"

USenseOverlapZoneComponent::USenseOverlapZoneComponent()
{
	SetGenerateOverlapEvents(true);
	
	OnComponentBeginOverlap.AddDynamic(this, &USenseOverlapZoneComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &USenseOverlapZoneComponent::OnEndOverlap);
}


void USenseOverlapZoneComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner()&& OtherActor->IsA<AMainCharacter>()) 
	{
		
		AActor* OwnerActor = GetOwner();
		
		if (OwnerActor)
		{
			
			UAISense_Damage::ReportDamageEvent(
				GetWorld(),
				OwnerActor,      
				OtherActor,      
				10.0f,          
				OwnerActor->GetActorLocation(), 
				OtherActor->GetActorLocation(),
				UCreatureEnumHelper::EnumToFName(EDamageAction::TriggerBoxDamage)
			);
		}
		
	}
}

// Gestore evento OnEndOverlap
void USenseOverlapZoneComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != GetOwner() && OtherActor->IsA<AMainCharacter>()) 
	{
		AActor* OwnerActor = GetOwner();
		
		if (OwnerActor)
		{
			UAISense_Damage::ReportDamageEvent(
				GetWorld(),
				OwnerActor,      
				OtherActor,      
				1.0f,          
				OwnerActor->GetActorLocation(), 
				OtherActor->GetActorLocation(),
				UCreatureEnumHelper::EnumToFName(EDamageAction::TriggerBoxDamage)
			);
		}
	}
}