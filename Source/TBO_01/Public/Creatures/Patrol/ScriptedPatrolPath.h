// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PatrolPath.h"
#include "ScriptedPatrolPath.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FScriptedAction
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	TSubclassOf<UGameplayAbility> Ability;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	float DelayBefore;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	float DelayAfter;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	bool Event;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	FGameplayTag EventTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	AActor* TargetActor;

	
	FScriptedAction()
	: Ability(nullptr)
	, DelayBefore(0.f)
	, DelayAfter(0.f)
	, Event(false)
	, TargetActor(nullptr)
	{}
};


UCLASS()
class TBO_01_API AScriptedPatrolPath : public APatrolPath
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FScriptedAction GetAction(int Position);

	UPROPERTY()
	bool IsCompleted = false;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TArray<FScriptedAction> ScriptedActions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	bool DrawDebug = false;
};
