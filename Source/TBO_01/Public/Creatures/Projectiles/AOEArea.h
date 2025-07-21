// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interface/EffectHandler.h"
#include "AOEArea.generated.h"

class UAbilitySystemComponent;
class USphereComponent;
class UNiagaraComponent;

UCLASS()
class TBO_01_API AAOEArea : public AActor, public IEffectHandler
{
	GENERATED_BODY()
	
public:
	AAOEArea();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AOE")
	UNiagaraComponent* NiagaraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AOE")
	USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AOE", Meta = (UIMin = -1f, ClampMin = -1f))
	float AOEDuration = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AOE", Meta = (UIMin = -1f, ClampMin = -1f))
	float AOEEndDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE|NiagaraSpawnRate", Meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float NiagaraInitialSpawnRate = 2000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE|NiagaraSpawnRate", Meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float SpawnRateFalloffSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE|NiagaraSpawnRate")
	FName NiagaraSpawnRateVariableName = "User.SpawnRate";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE|Radius", Meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float AOETargetRadius = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE|Radius", Meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float RadiusChangeSpeed = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE|Radius")
	FName NiagaraRadiusVariableName = "User.SpreadRadius";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE DOT GAS")
	FGameplayTag AOEDOTAbilityTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE DOT GAS")
	FGameplayTag AOEDOTEffectAbilityTag;

	UFUNCTION(BlueprintCallable, Category = "AOE")
	void EndAOE();
	UFUNCTION(BlueprintCallable, Category = "AOE")
	void CompleteEndAOE();
	UFUNCTION(BlueprintCallable, Category = "AOE")
	void RemoveActiveEffects(UAbilitySystemComponent* TargetASC);
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void ApplyEffect(AActor* OtherActor);
	UFUNCTION()
	void RemoveEffect(AActor* OtherActor);
	UFUNCTION()
	void ApplyEffectsToOverlappingActors();

	
	float CurrentRadius = 0.0f;
	float CurrentSpawnRate = 0.0f;
	FTimerHandle AOEDurationTimerHandle;
	FTimerHandle AOEEndDurationTimerHandle;
	bool AOEEnding = false;
	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> ActiveEffectsMap;

/**Reapply effect*/
	FTimerHandle AOETickTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "AOE")
	float AOEInterval = 1.0f;
	
	UPROPERTY()
	TSet<AActor*> OverlappingActors;
	
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float AOEDamage = 0.5f;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyEffect_Implementation(UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle ActiveEffectHandle) override;
};


inline void AAOEArea::ApplyEffectsToOverlappingActors()
{
	for (AActor* Actor : OverlappingActors)
	{
		if (IsValid(Actor))
		{
			ApplyEffect(Actor);
		}
	}
}
