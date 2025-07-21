// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GameplayObjectState.h"
#include "ActuatorActor.generated.h"

class USaveSubsystem;
class UUniqueIDComponent;
class AActionableActor;

UCLASS()
class TBO_01_API AActuatorActor : public AActor, public IGameplayObjectState
{
	GENERATED_BODY()

protected:
	AActuatorActor();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ActionableActor")
	USceneComponent* MyRoot = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorComponent")
	UUniqueIDComponent* UniqueIDComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connected Actionable Actors")
	TArray<TObjectPtr<AActionableActor>> ConnectedActionableActor;
	virtual void RestoreState_Implementation() override;
	virtual bool HasBeenActivated_Implementation() override;

	UPROPERTY()
	USaveSubsystem* SaveSubsystem = nullptr;

	bool Activated = false;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void ActionActors(const bool InstantActivation = false);
	UFUNCTION(BlueprintCallable)
	virtual void DeActivateActors(const bool InstantDeActivation = false);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRestore();
};
