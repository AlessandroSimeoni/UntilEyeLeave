// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActionableActor.h"
#include "DoubleDoor.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API ADoubleDoor : public AActionableActor
{
	GENERATED_BODY()

protected:
	ADoubleDoor();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DoubleDoor")
	bool IsOpen = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DoubleDoor")
	UStaticMeshComponent* WallMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DoubleDoor")
	USceneComponent* RightDoorPivot = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DoubleDoor")
	USceneComponent* LeftDoorPivot = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DoubleDoor")
	UStaticMeshComponent* RightDoorMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DoubleDoor")
	UStaticMeshComponent* LeftDoorMesh = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "SingleDoor")
	bool CurrentlyOpen = false;

	UFUNCTION(BlueprintCallable, Category = "SingleDoorFunction")
	void InstantStateChange(bool Open);

	virtual void PostInitializeComponents() override;
	virtual void ActivateActor_Implementation(bool InstantActivation) override;
	virtual void DeActivateActor_Implementation(bool InstantDeActivation) override;
};
