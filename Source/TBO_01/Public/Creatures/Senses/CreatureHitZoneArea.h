// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneComponent/HittableCollisionBoxComponent.h"
#include "Creatures/CreatureEnumHelper.h"
#include "CreatureHitZoneArea.generated.h"


UCLASS(ClassGroup=(Hittable), meta=(BlueprintSpawnableComponent))
class TBO_01_API UCreatureHitZoneArea : public UHittableCollisionBoxComponent
{
	GENERATED_BODY()

public:
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator,float Intensity = 1.f) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	EDamageAction DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	int ResistanceLevel = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	bool PlayerOnly = true;
	
};
