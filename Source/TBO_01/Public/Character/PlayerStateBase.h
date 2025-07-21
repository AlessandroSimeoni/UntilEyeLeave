// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM/StateBase.h"
#include "PlayerStateBase.generated.h"

class UCharacterAnimInstance;
class ACharacterController;
class AMainCharacter;
/**
 * This is the base class for the PLAYER states
 */
UCLASS()
class TBO_01_API UPlayerStateBase : public UStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMainCharacter> MainCharacter = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacterController> CharacterController = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterAnimInstance> CharacterAnimInstance = nullptr;

public:
	virtual void InitState(UStateController* Controller) override;
	
};
