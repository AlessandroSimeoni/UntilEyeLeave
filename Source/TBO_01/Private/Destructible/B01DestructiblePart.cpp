// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible/B01DestructiblePart.h"


#include "Character/MainCharacter.h"
#include "Perception/AISense_Damage.h"

bool UB01DestructiblePart::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity)
{
	//Super::Hit_Implementation(CharacterLevel, DamageValue, AttackInstigator);
	UE_LOG(LogTemp, Warning, TEXT("Hitzone AreaHitted"));

	AActor* OwnerActor = GetOwner();

	//destroy shell
	if (DestructionResistanceLevel <= CharacterLevel)
	{
	
		if (IsValid(OwnerActor))
		{
			UAISense_Damage::ReportDamageEvent(
				GetWorld(),
				OwnerActor,      
				AttackInstigator,      
				DamageValue,          
				OwnerActor->GetActorLocation(), 
				AttackInstigator->GetActorLocation(),
				UCreatureEnumHelper::EnumToFName(DestructionDamageType)
			);
		}
		StartDestruction();
		return true;
	}

	if (!AttackInstigator->IsA<AMainCharacter>() && PlayerOnly)
		return false;

	//Activate character
	if (ResistanceLevel > CharacterLevel)
		return false;

	if (IsValid(OwnerActor))
	{
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			OwnerActor,      
			AttackInstigator,      
			DamageValue,          
			OwnerActor->GetActorLocation(), 
			AttackInstigator->GetActorLocation(),
			UCreatureEnumHelper::EnumToFName(HitDamageType)
		);
		return true;
	}
	
	return false;
}
