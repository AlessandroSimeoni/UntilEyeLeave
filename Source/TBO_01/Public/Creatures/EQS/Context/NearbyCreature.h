// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "NearbyCreature.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UNearbyCreature : public UEnvQueryContext
{
	GENERATED_BODY()
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	UPROPERTY(EditDefaultsOnly,Category="EQS|Context")
	float SearchRadius;
};
