// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/EffectData.h"
#include "AOEDamage.generated.h"

UCLASS()
class TBO_01_API AAOEDamage : public AActor
{
	GENERATED_BODY()
	
public:	
	AAOEDamage();
	UFUNCTION(BlueprintCallable, Category="AOE")
	void Explode(float DestructionLevel,float Damage,float Size, float KnockabackIntensity = 1.f);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="FX")
	FEffectData ExplosionFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AOE")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="AOE")
	bool DebugHitBox =false;
	


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	FName NiagaraRadiusVariableName = "User.SpreadRadius";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	float FXExplosionRadiusMultiplier = 1;

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> HittableActors;
	UPROPERTY()
	TArray<TObjectPtr<UPrimitiveComponent>> HittableComponents;
	FTimerHandle AOEDurationTimerHandle;
	
	void EndAOE();

};

