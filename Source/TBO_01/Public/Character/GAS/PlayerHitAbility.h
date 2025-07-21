// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NativeGameplayTags.h"
#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerCombatState.h"
#include "PlayerHitAbility.generated.h"

/**
 * 
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DeflectHit);
UCLASS()
class TBO_01_API UPlayerHitAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPlayerHitAbility();
protected:
	virtual void ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	TSubclassOf<UPlayerStateBase> CombatStateClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tag")
	FName DeflectTag = FName("Deflect");


private:
	UPROPERTY()
	TObjectPtr<AMainCharacter> MyCharacter;
	UPROPERTY()
	TObjectPtr<UPlayerCombatState> CombatState;
	UPROPERTY()
	const class UPlayerAttributeSet* AttributesSet;
};
