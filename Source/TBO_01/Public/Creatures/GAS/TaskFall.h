// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TaskFall.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFallFinished);
UCLASS()
class TBO_01_API UTaskFall : public UAbilityTask
{
	GENERATED_BODY()
	public:
	
	UPROPERTY(BlueprintAssignable)
	FOnFallFinished OnFallFinished;
	
	UTaskFall(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Fall Character", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UTaskFall* Fall(UGameplayAbility* OwningAbility, FName TaskInstanceName);

	virtual void Activate() override;
protected:
	virtual void OnDestroy(bool AbilityEnded) override;

	UFUNCTION()
	void OnMovementModeChange(ACharacter* Character, EMovementMode PrevMode, uint8 PrevCustomMode);
	
private:
	UPROPERTY()
	TObjectPtr<AActor> OwningAbilityTargetActor;
};
