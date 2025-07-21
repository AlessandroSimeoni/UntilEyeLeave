// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NearPosition.h"
#include "BehaviorTree/BTService.h"
#include "NearScriptedPosition.generated.h"

/**
 * 
 */


UCLASS()
class TBO_01_API UNearScriptedPosition : public UNearPosition
{
	GENERATED_BODY()

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	


};
