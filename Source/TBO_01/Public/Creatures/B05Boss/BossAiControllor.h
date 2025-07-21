// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAiControllor.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class TBO_01_API ABossAiControllor : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "BossAiControllor")
	bool SetPlayerTarget();
	
protected:
	
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameHasPlayer = "Player";

private:
	UFUNCTION()
	virtual void PlayerDeath(USceneComponent* PlayerLifeComponent);

};
