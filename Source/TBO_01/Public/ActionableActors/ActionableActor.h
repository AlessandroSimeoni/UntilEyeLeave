// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActivationOver);

UCLASS(Abstract, Blueprintable)
class TBO_01_API AActionableActor : public AActor
{
	GENERATED_BODY()

protected:
	AActionableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ActionableActor")
	USceneComponent* MyRoot = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionableActor", Meta = (UImin = 1, ClampMin = 1))
	int ActivationRequestsRequired = 1;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateActor(const bool InstantActivation = false);
	virtual void ActivateActor_Implementation(bool InstantActivation = false);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeActivateActor(const bool InstantDeActivation = false);
	virtual void DeActivateActor_Implementation(bool InstantDeActivation = false);
	
	int CurrentActivationRequests = 0;
	bool Activated = false;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void RequestActivation(const bool InstantActivation);
	UFUNCTION(BlueprintCallable)
	virtual void RequestDeActivation(const bool InstantDeActivation);
	
	FActivationOver OnActivationOver; 
};
