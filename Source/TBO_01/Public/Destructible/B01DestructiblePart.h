// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Destructible/DestructibleParts.h"
#include "Creatures/CreatureEnumHelper.h"
#include "B01DestructiblePart.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Hittable), meta=(BlueprintSpawnableComponent))
class TBO_01_API UB01DestructiblePart : public UDestructibleParts
{
	GENERATED_BODY()
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator,float Intensity = 1.f) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	int DestructionResistanceLevel = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	EDamageAction HitDamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	EDamageAction DestructionDamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	bool PlayerOnly = true;
};
