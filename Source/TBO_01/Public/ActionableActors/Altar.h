// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActuatorActor.h"
#include "Altar.generated.h"

class AB04_Controller;
class AB04_CageBird;
class UNiagaraComponent;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class TBO_01_API AAltar : public AActuatorActor
{
	GENERATED_BODY()

protected:

	AAltar();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altar")
	UBoxComponent* BoxTriggerArea = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altar")
	UStaticMeshComponent* AltarMeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altar")
	UNiagaraComponent* HintVFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altar")
	USkeletalMeshComponent* BirdSkeletalMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altar")
	UMaterialInterface* ActiveAltarMaterial = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altar")
	USoundBase* AltarSound = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altar")
	TSubclassOf<AB04_CageBird> BirdClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altar")
	USceneComponent* BirdPosition = nullptr;

	TObjectPtr<AB04_Controller> BirdController = nullptr;

	void SetActiveMeshStateVisual() const;
	UFUNCTION()
	void OnAltarAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleBirdReachedAltar();

	virtual void BeginPlay() override;
	void ClearTriggerArea();
	virtual void RestoreState_Implementation() override;
	virtual void ActionActors(const bool InstantActivation = false) override;
};
