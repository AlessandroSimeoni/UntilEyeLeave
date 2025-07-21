// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayAbilitySpecHandle.h"
#include "Patrol/ScriptedPatrolPath.h"
#include "ScriptedController.generated.h"

struct FScriptedAction;
class ACreature;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScriptedPatroleCompleted);

UCLASS()
class TBO_01_API AScriptedController : public AAIController
{
	GENERATED_BODY()
	


public:
	// Sets default values for this actor's properties
	AScriptedController();

	UPROPERTY(BlueprintAssignable, Category="Spline")
	FOnScriptedPatroleCompleted OnScriptedPatrolCompleted;

	bool bPathCompleted = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACreature> PawnCreature;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameStop = "Stop";
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameIsPatrolPathAssigned = "IsPatrolPathAssigned";
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNamePosition = "Position";
	
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly)
	bool bResetPathOnEnd = false;
	
	UFUNCTION()
	void DoScriptedAction(const FScriptedAction& ScriptedAction);
	UFUNCTION()
	void ActivateAbility(const FScriptedAction& ScriptedAction);
	UFUNCTION(BlueprintCallable)
	void SetStop(bool Stop);
	
private:
	UFUNCTION()
	void OnAbilityEndedCallback(const FAbilityEndedData& Data);

	UFUNCTION()
	void ResetPath();
	
	FScriptedAction CurrentScriptedAction;
	FGameplayAbilitySpecHandle ActiveAbilityHandle;

	FTimerHandle CurrentTimer;
	
	
};

