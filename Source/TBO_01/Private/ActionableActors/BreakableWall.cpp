// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/BreakableWall.h"

bool ABreakableWall::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator,float Intensity)
{
	UE_LOG(LogTemp, Warning, TEXT("HittableDestructibleActor::Hit_Implementation %d"),CharacterLevel);

	if (!HitClasses.Contains(AttackInstigator->GetClass()))
		return false;
	
	if (ResistanceLevel > CharacterLevel)
	{
		return false; 
	}
	
	Destruction();
	return true;
}
