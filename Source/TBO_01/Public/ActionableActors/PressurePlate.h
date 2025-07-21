// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActuatorActor.h"
#include "Creatures/CreatureEnumHelper.h"
#include "PressurePlate.generated.h"

class UAbilitySystemComponent;
class AShooterCreature;
class UPressurePlateAnimInstance;
class ACreature;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class TBO_01_API APressurePlate : public AActuatorActor
{
	GENERATED_BODY()

protected:

	APressurePlate();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PressurePlate")
	UBoxComponent* BoxTriggerArea = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PressurePlate")
	USkeletalMeshComponent* PressurePlateSkeletalMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PressurePlate")
	TSubclassOf<AShooterCreature> DestructibleGianguscioClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PressurePlate")
	UAnimMontage* DeactivatePressureAM = nullptr;
	UPROPERTY()
	UPressurePlateAnimInstance* PressurePlateAnimInstance = nullptr;
	
	int CurrentOverlappedActors = 0;

	UFUNCTION()
	void HandleCreatureEndOverlap(AActor* Actor,const AActor* DeathInstigator = nullptr);
	UFUNCTION()
	void OnPressurePlateBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPressurePlateEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void HandleBrokenShellDamage(AActor* AttackerActor, AActor* Receiver, EDamageAction DamageType, float DamageValue);
	UFUNCTION(BlueprintImplementableEvent)
	void MoveGroundCollisions(bool Activation);
	
	bool HasBrokenShell(const UAbilitySystemComponent* TargetASC);
	bool HasBrokenShell(const AActor* TargetActor);
	
	virtual void BeginPlay() override;
	virtual void ActionActors(const bool InstantActivation = false) override;
	virtual void DeActivateActors(const bool InstantDeActivation = false) override;
};
