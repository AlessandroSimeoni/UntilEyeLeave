// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Destructible/HittableDestructibleActor.h"
#include "BreakableWall.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API ABreakableWall : public AHittableDestructibleActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BreakableWall")
	TArray<TSubclassOf<AActor>> HitClasses;
	
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity = 1.f) override;
};
