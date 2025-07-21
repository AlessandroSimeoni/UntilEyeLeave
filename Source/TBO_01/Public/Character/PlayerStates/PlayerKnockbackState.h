// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerStateBase.h"
#include "PlayerKnockbackState.generated.h"

class UGameplayAbility;
class UPlayerIdleState;
/**
 * 
 */
UCLASS()
class TBO_01_API UPlayerKnockbackState : public UPlayerStateBase
{
	GENERATED_BODY()

	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> IdleStateClass;

	UPROPERTY()
	UPlayerIdleState* IdleState = nullptr;
	UPROPERTY()
	UGameplayAbility* KnockbackAbilityInstance = nullptr;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knockback Function")
	void StartKnockback();

	UFUNCTION(BlueprintCallable, Category = "Ledge Grab Function")
	void EndKnockback();
};
