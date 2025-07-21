// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActuatorActor.h"
#include "Eyes/EyesHandlerInterface.h"
#include "EyeButton.generated.h"

class UEyeSceneComponent;
/**
 * 
 */
UCLASS()
class TBO_01_API AEyeButton : public AActuatorActor, public IEyesHandlerInterface
{
	GENERATED_BODY()

protected:
	AEyeButton();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eye Button")
	UStaticMeshComponent* SocketMeshComponent = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eye Button")
	TObjectPtr<UEyesHandlerActorComponent> EyeHandlerActorComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Eye Button")
	TObjectPtr<USkeletalMeshComponent> EyeBody;

	UPROPERTY()
	TArray<TObjectPtr<UEyeSceneComponent>> Eyes;
	FTimerHandle TH;

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleAlreadyOverlappingPlayer();
	UFUNCTION(BlueprintCallable)
	void HandleEyeHit(UEyeSceneComponent* TargetEyeComponent, float Damage, const AActor* HitInstigator, float Force);
	virtual UEyesHandlerActorComponent* GetEyesHandlerActorComponent_Implementation() override;
};
