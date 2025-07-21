// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActionableActor.h"
#include "StopLight.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API AStopLight : public AActionableActor
{
	GENERATED_BODY()

protected:

	AStopLight();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StopLight")
	UStaticMeshComponent* StopLightMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StopLight")
	UMaterialInterface* ActiveMaterial = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StopLight")
	UMaterialInterface* DisableMaterial = nullptr;

	virtual void ActivateActor_Implementation(bool InstantActivation) override;
	virtual void DeActivateActor_Implementation(bool InstantDeActivation) override;
};
