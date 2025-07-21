// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActionableActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API AMovingPlatform : public AActionableActor
{
	GENERATED_BODY()

protected:

	AMovingPlatform();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MovingPlatform")
	bool IsUp = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovingPlatform")
	USceneComponent* TopPosition = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovingPlatform")
	USceneComponent* BottomPosition = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovingPlatform")
	USceneComponent* MovingPart = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovingPlatform")
	UStaticMeshComponent* PlatformMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MovingPlatform")
	TArray<AActor*> AttachedActors;
	UPROPERTY(BlueprintReadWrite, Category = "MovingPlatform")
	bool CurrentlyUp = false;
	
	UFUNCTION(BlueprintCallable, Category = "MovingPlatformFunction")
	void AttachActorsToPlatform();
	UFUNCTION(BlueprintCallable, Category = "MovingPlatformFunction")
	void InstantStateChange(bool Up);

	virtual void PostInitializeComponents() override;
	virtual void PostRegisterAllComponents() override;
	virtual void ActivateActor_Implementation(bool InstantActivation = false) override;
	virtual void DeActivateActor_Implementation(bool InstantDeActivation = false) override;
	
};
