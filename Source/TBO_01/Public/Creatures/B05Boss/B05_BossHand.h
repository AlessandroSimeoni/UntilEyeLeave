// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/Creature.h"
#include "B05_BossHand.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API AB05_BossHand : public ACreature
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FonStatusChange,bool, bStatus);
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable,Category="Boss")
	FonStatusChange OnStatusChange;
	
	void SetRestPoint(FVector RestPoint);
	UFUNCTION(BlueprintCallable)
	void SetActiveStatus(bool Status);

protected:
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameRestPosition = "RestPosition";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameHandActive = "HandActive";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameStop= "Stop";

	UPROPERTY(BlueprintReadWrite,Category="Boss",meta=(AllowPrivateAccess=true))
	bool StatusActive = false;
	
};
