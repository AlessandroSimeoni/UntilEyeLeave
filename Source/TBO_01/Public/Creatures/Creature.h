// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "Eyes/EyeSceneComponent.h"
#include "Eyes/EyesHandlerActorComponent.h"
#include "Eyes/EyesHandlerInterface.h"
#include "Patrol/Interface/PatrolInterface.h"
#include "Creature.generated.h"

class UCoinContainer;
class ACreatureAiController;
class UDamageFlash;
struct FEffectData;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Death);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreatureDeath, AActor*, Actor, const AActor*, Instigator);

UCLASS()
class TBO_01_API ACreature : public ACharacter, public IAbilitySystemInterface,public IPatrolInterface, public IEyesHandlerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	
	ACreature();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	/**Event*/
	UPROPERTY(BlueprintAssignable, Category="Creature")
	FOnCreatureDeath OnDeath;

	
	
	
	/**Hit response Function*/
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, bool bSelfMoved,
					   FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	/**Abilty Sistem*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS",meta=(AllowPrivateAccess=true))
	const class UBaseCreatureAttributeSet* BasicsAttributesSet;

	//**Patrol spline interface*/
	UFUNCTION(BlueprintCallable,Category="Patrol")
	virtual UPatrolSplineComponent* GetPatrolSplineComponent_Implementation() override;

	UFUNCTION(BlueprintCallable,Category="Eye")
	virtual UEyesHandlerActorComponent* GetEyesHandlerActorComponent_Implementation() override;

	//Navigation Influence
	UFUNCTION(BlueprintCallable,Category="Navigation")
	void SetEffectNavigation(bool CanEffect);

	bool bOptimizationTickEnabled = true;
	void SetOptimizationTick(bool bIsInRange);
	
	UFUNCTION(BlueprintCallable,Category="Creature")
	void InstantDeath();
	UFUNCTION(BlueprintCallable,Category="Creature")
	void Spawn();
	UFUNCTION(BlueprintCallable,Category="Creature")
	void Death(bool PlayFx = true);
	void DelayedDestroy();
	
	/**Movement Component*/
	UFUNCTION(BlueprintCallable,Category="Movement component")
	void SwitchMovementMode(bool bUseDesiredRotationMode);
	
	UFUNCTION(BlueprintCallable,Category="Movement component")
	void Teleport(const FVector& Position, EMovementMode Mode);
	
	void AlignToGround(float DeltaTime);

	TWeakObjectPtr<ACreatureAiController> CreatureAiController;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Patrol")
	TObjectPtr<UCoinContainer> CoinContainer;
	

protected:

	//eyes event 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Eye")
	void OnEyeHit(UEyeSceneComponent* Eye, float Damage, const AActor* ActorInstigator, float Force);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Eye")
	void OnEyeDeath(UEyeSceneComponent* Eye,const AActor* ActorInstigator);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Eye")
	void OnAllEyesDeath(UEyeSceneComponent* Eye, const AActor* ActorInstigator);

	UFUNCTION()
	void AllEyesDeath(UEyeSceneComponent* Eye, const AActor* ActorInstigator);
	
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	UPROPERTY()
	TArray<TObjectPtr<UEyeSceneComponent>> Eyes;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly,Category="Gas")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbility;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Patrol")
	TObjectPtr<UPatrolSplineComponent> PatrolSplineComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Patrol")
	TObjectPtr<UEyesHandlerActorComponent> EyesHandlerActorComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Tick")
	bool bAddTickAggregator = true;

	
	/**Effects*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FX")
	FEffectData DeathFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FX")
	FEffectData SpawnFX;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="FX")
	TObjectPtr<UDamageFlash> DamageFlashComponent;

	/**Alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement component")
	bool IsAlignedToGround = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement component")
	float AlignmentSpeed = 5.0f;
	FVector LastValidFloorNormal = FVector::UpVector;
	float LastYaw = 0.0f;
	FRotator CachedTargetRotation = FRotator::ZeroRotator;

	
	
private:
	UFUNCTION()
	void GiveDefaultAbility();
	UPROPERTY()
	const AActor* Killer;

	FVector StartPosition;
	
};


