// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "Character/PlayerStateBase.h"
#include "Utility/EffectData.h"
#include "PlayerCombatState.generated.h"

class UPlayerRollState;
class UPlayerKnockbackState;
class UPlayerLedgeFallingState;
class UPlayerIdleState;
/**
 * 
 */
UCLASS()
class TBO_01_API UPlayerCombatState : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;

	void SetAttackReady();
	
	float ForwardMidAirControlFactor = 0.0f;
	float HorizontalMidAirControlFactor = 0.0f;

	bool IsHitSuccess = false;
	bool IsDeflect = false;
	
	UFUNCTION()
	void Hit(const AActor* HittedActor, const UPrimitiveComponent* CollisionBox, FVector Location);
	UFUNCTION()
	void Deflect(const AActor* HittedActor, const UPrimitiveComponent* CollisionBox, FVector Location);
	UFUNCTION()
	void ResetTimeDilation();
	UFUNCTION()
	void StartTimeDilation();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> IdleStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> LedgeFallingStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> KnockbackStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> RollStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat|Mid Air", Meta = (UIMin = 0.0f, UIMax = 1.0f, ClampMin = 0.0f, ClampMax = 1.0f))
	float MidAirGravityWeight = 1.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat|Mid Air", Meta = (UIMin = 0.0f, UIMax = 1.0f, ClampMin = 0.0f, ClampMax = 1.0f))
	float MidAirControlWeight = 1.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat|Settings", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float AnimationPlayRate = 1.0f;

	/**Hitstop*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat|HitStop")
	float HitStopTime = 0.1f;
	FTimerHandle TimeDilationResetTimer;
	/**GAS*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	FGameplayTag DeflectEventTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	FGameplayTag ComboWindowOpenTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	FGameplayTag CommitAttackTag;
	
	/**FX*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
	FEffectData HitFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
	FEffectData DeflectFX;
	
	UPROPERTY()
	UPlayerIdleState* IdleState = nullptr;
	UPROPERTY()
	UPlayerLedgeFallingState* LedgeFallingState = nullptr;
	UPROPERTY()
	UPlayerKnockbackState* KnockbackState = nullptr;
	UPROPERTY()
	UPlayerRollState* RollState = nullptr;
	
	FTimerHandle AttackCooldownTimer;
	FTimerManager* TM; 
	float PreviousGravityScale = 0.0f;

	/**Input Buffer*/
	bool ValidInputBuffer = false;
	float InputBufferTimeStamp = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputBuffer")
	float InputBufferDuration = 0.2f;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack Function")
	void StartAttack();
	UFUNCTION(BlueprintCallable, Category = "Attack Function")
	void EndAttack() const;
	UFUNCTION()
	virtual void HandleMidAirMovement(FVector MovementDirection);
	UFUNCTION()
	void GoToKnockbackState();
	UFUNCTION()
	void AttackInputHandle();
	UFUNCTION()
	void HandleCharacterRotation(FVector MoveDirection);
	UFUNCTION()
	void HandleRollRequest();
	void SetGroundedBehavior();

	/**GAS*/
	void ComboWindowOpen(const FGameplayEventData* Payload);
	void ConsumeBuffer(const FGameplayEventData* Payload);
	void ResetCanRotate(const FGameplayEventData* Payload);

	bool CanRotate = true;
	float CharacterMaxSpeed  = 0.0f;;
};
