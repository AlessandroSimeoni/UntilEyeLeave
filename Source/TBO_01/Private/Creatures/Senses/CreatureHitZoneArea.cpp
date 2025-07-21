// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Senses/CreatureHitZoneArea.h"

#include "Character/MainCharacter.h"
#include "Perception/AISense_Damage.h"




bool UCreatureHitZoneArea::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity)
{
	//Super::Hit_Implementation(CharacterLevel, DamageValue, AttackInstigator);
	UE_LOG(LogTemp, Warning, TEXT("Hitzone AreaHitted"));

	if (!AttackInstigator->IsA<AMainCharacter>() || !PlayerOnly)
		return false;
		
	
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
			UCreatureEnumHelper::EnumToFName(DamageType)
		);
		return true;
	}
	return false;
}


