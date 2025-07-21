// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStateBase.h"

#include "Character/CharacterAnimInstance.h"
#include "Character/CharacterController.h"
#include "Character/MainCharacter.h"
#include "FSM/StateController.h"

void UPlayerStateBase::InitState(UStateController* Controller)
{
	Super::InitState(Controller);

	// retrieve the character and cache it
	if (StateController)
		MainCharacter = Cast<AMainCharacter>(StateController->GetOwner());

	if (MainCharacter)
	{
		CharacterController = Cast<ACharacterController>(MainCharacter->GetController());
		CharacterAnimInstance = Cast<UCharacterAnimInstance>(MainCharacter->GetMesh()->GetAnimInstance());
	}
}
