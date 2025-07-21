// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActuatorActor.h"
#include "Interface/HittableActor.h"
#include "BulletTarget.generated.h"

class ABaseProjectile;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class TBO_01_API ABulletTarget : public AActuatorActor, public IHittableActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="StreetLamp")
	void OnTargetHit();
protected:

	ABulletTarget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BulletTarget")
	UStaticMeshComponent* TargetMeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BulletTarget")
	UNiagaraComponent* HintVFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BulletTarget")
	TArray<TSubclassOf<ABaseProjectile>> ProjectileClasses;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BulletTarget")
	UMaterialInterface* ActiveTargetMaterial = nullptr;
	
	void ChangeMaterial() const;
	
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator,float Intensity = 1.f) override;
	virtual void RestoreState_Implementation() override;
	virtual void ActionActors(const bool InstantActivation = false) override;
};
