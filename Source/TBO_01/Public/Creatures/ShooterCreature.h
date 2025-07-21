// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/Creature.h"
#include "Interface/Shooter.h"
#include "ShooterCreature.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API AShooterCreature : public ACreature, public IShooter
{
	GENERATED_BODY()
	
	
	UFUNCTION(BlueprintCallable,Category="Muzzle")
	virtual UMuzzlesController* GetMuzzlesController_Implementation() override;

protected:
	AShooterCreature();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Muzzle")
	TObjectPtr<UMuzzlesController> MuzzleController;
	
};
