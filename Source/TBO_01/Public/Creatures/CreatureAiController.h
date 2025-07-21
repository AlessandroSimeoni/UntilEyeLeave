// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "NativeGameplayTags.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "Perception/AISenseConfig_Touch.h"
#include "Senses/ComplexCreatureSight.h"
#include "TeamAffiliationConfig.h"
#include "CreatureAiController.generated.h"

/**
 * 
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Spotted);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Lost);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorSpotted,AActor*, SpottedActor,ETeamAttitude::Type, Attitude);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorHeared,AActor*, HearedActor,ENoiseAction, NoiseType,bool,IsValid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnActorDamage,AActor*, AttackerActor,AActor*, Reciver, EDamageAction, DamageType, float, DamageValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTriggerBoxDamage,AActor*, AttackerActor,EDamageAction, DamageType, float, DamageValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorTouched,AActor*, TouchingActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorLost,AActor*, LostActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpottedActorChangeAffiliation,AActor*, SpottedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPredictionActorMovement, FVector, PredictedLocation, AActor*, PredictedActor);
UCLASS()
class TBO_01_API ACreatureAiController : public AAIController
{
	GENERATED_BODY()

public:
	ACreatureAiController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
//const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()
	
	
	UPROPERTY(BlueprintAssignable, Category = "Creatures|Sense|Sight")
	FOnSpottedActorChangeAffiliation OnSpottedActorChangeAffiliation;
	UPROPERTY(BlueprintAssignable, Category = "Creatures|Sense|Sight")
	FOnActorSpotted OnActorSpotted;
	UPROPERTY(BlueprintAssignable, Category = "Creatures|Sense|Sight")
	FOnActorLost OnActorLost;
	UPROPERTY(BlueprintAssignable, Category = "Creatures|Sense|Hearing")
	FOnActorHeared OnActorHeared;
	UPROPERTY(BlueprintAssignable, Category = "Creatures|Sense|Touch")
	FOnActorTouched OnActorTouched;
	UPROPERTY(BlueprintAssignable, Category = "Creatures|Sense|Prediction")
	FOnPredictionActorMovement OnPredictionActorMovement;
	UPROPERTY(BlueprintAssignable, Category = "Creatures|Sense|Damage")
	FOnActorDamage OnActorDamage;
	UPROPERTY(BlueprintAssignable, Category = "Creatures|Sense|Damage")
	FTriggerBoxDamage OnTriggerBoxDamage;
	/** Sense component reference*/
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Creatures|Sense")
	TObjectPtr<UComplexCreatureSight> ComplexCreatureSightComponent;
	
protected:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	//BlueprintImplementableEvent
	UFUNCTION(BlueprintImplementableEvent, Category = "Creatures|Sense|Sight")
	void ActorSpotted(AActor* SpottedActor, ETeamAttitude::Type Attitude);
	UFUNCTION(BlueprintImplementableEvent, Category = "Creatures|Sense|Hearing")
	void ActorHeared(AActor* HearedActor, ENoiseAction NoiseType,bool IsValid);
	UFUNCTION(BlueprintImplementableEvent, Category = "Creatures|Sense|Damage")
	void ActorDamage(AActor* Attacker, EDamageAction DamageType, float DamageValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "Creatures|Sense|Damage")
	void TriggerBoxDamage(AActor* Attacker, EDamageAction DamageType, float DamageValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "Creatures|Sense|Touch")
	void ActorTouched(AActor* TouchingActor);
	UFUNCTION(BlueprintImplementableEvent, Category = "Creatures|Sense|Sight")
	void ActorLost(AActor* LostActor);
	UFUNCTION(BlueprintImplementableEvent, Category = "Creatures|Sense|Sight")
	void SpottedActorChangeAffiliation(AActor* SpottedActor);
	UFUNCTION(BlueprintImplementableEvent, Category = "Creatures|Sense|Prediction")
	void PredictionActorMovement(FVector PredictedLocation, AActor* PredictedActor,bool IsValid);

	//** Set Eye follow */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Creatures|Eye")
	bool IsSetEyeFollow = false;
	
	/** Blackboard ID*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameHasPlayer = "Player";
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameHasPredictedPosition = "PredictedPosition";
	
	/** Sense Property */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Creatures|Sense")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	
	UPROPERTY(Transient)
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	UPROPERTY(Transient)
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	UPROPERTY(Transient)
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	UPROPERTY(Transient)
	TObjectPtr<UAISenseConfig_Touch> TouchConfig;
	UPROPERTY(Transient)
	TObjectPtr<UAISenseConfig_Prediction> PredictionConfig;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Creatures|Sense|Hearing")
	bool TouchEventOnlyWithPlayer;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Creatures|Sense|Prediction")
	float PredictionTime = 0.5f;
	
	
	/**Affiliation*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Affiliaton")
	FTeamAffiliationConfig TeamAffiliationConfig;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	

	
	
	//Function
	/** Sense Function */
	UFUNCTION()
	void OnPerceptionUpdate(AActor * Actor, FAIStimulus   Stimulus);

	/** function that handle the sight add Spotted Actor to the component*/
	UFUNCTION(BlueprintCallable,Category = "Creature|Sense|Sight")
	virtual void HandleSight(bool IsSpotted, AActor * Actor);

	/**Function that handle the Hearing sense*/
	UFUNCTION(BlueprintNativeEvent,Category = "Creatures|Sense|Hearing")
	void HandleHearing(AActor * Actor,ENoiseAction NoiseType,bool IsValid);

	/**Function that handle the Touch no Tag*/
	UFUNCTION(BlueprintNativeEvent,Category = "Creatures|Sense|Touch")
	void HandleTouch(AActor * Actor);

	/**Function that handle the Hearing sense*/
	UFUNCTION(BlueprintNativeEvent,Category = "Creatures|Sense|Hearing")
	void HandleDamage(AActor * Actor,EDamageAction DamageType, float damageValue);

	
	/** return a vector and the actor that is currently predicted*/
	UFUNCTION(BlueprintNativeEvent,Category = "Creatures|Sense|Prediction")
	void HandlePrediction(const FVector &ActorPredictedPosition, AActor * PredictedActor, bool IsValid);
	
	//** set Team Affiliation*/
	UFUNCTION(Blueprintable,Category="Creature|Affiliation")
	void SetTeam(EGameTeam Team);

	//** Set Eye follow */
	
	UFUNCTION(Blueprintable,Category="Creatures|Eye")
	void SetEyesTarget(AActor* Target);
	UFUNCTION(Blueprintable,Category="Creatures|Eye")
	void RemoveEyesTarget(AActor* Target);

private:
	
	/**send event on actor spotted that change spotted status*/
	void SpottedActorChangeStatus(bool IsSpotted, FActorSpotted SpottedActor);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
};


