// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActionableActor.h"
#include "SingleDoor.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API ASingleDoor : public AActionableActor
{
	GENERATED_BODY()

protected:

	ASingleDoor();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SingleDoor")
	bool IsOpen = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SingleDoor")
	UStaticMeshComponent* WallMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SingleDoor")
	USceneComponent* DoorPivot = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SingleDoor")
	UStaticMeshComponent* DoorMesh = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "SingleDoor")
	bool CurrentlyOpen = false;
	
	UFUNCTION(BlueprintCallable, Category = "SingleDoorFunction")
	void InstantStateChange(bool Open);

	virtual void PostInitializeComponents() override;
	virtual void ActivateActor_Implementation(bool InstantActivation = false) override;
	virtual void DeActivateActor_Implementation(bool InstantDeActivation = false) override;
};
