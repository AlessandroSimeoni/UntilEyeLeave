// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UniqueIDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TBO_01_API UUniqueIDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void OnComponentCreated() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid UniqueID;
};
