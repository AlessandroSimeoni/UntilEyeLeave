// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerCombatState.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Character/CharacterController.h"
#include "Character/MainCharacter.h"
#include "TimerManager.h"
#include "Character/PlayerStates/PlayerIdleState.h"
#include "Character/PlayerStates/PlayerJumpState.h"
#include "Character/PlayerStates/PlayerKnockbackState.h"
#include "Character/PlayerStates/PlayerLedgeFallingState.h"
#include "Character/PlayerStates/PlayerMidAirState.h"
#include "Character/PlayerStates/PlayerRollState.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerCombatState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);

	IdleState = Cast<UPlayerIdleState>(StateController->FindStateOfClass(IdleStateClass));
	LedgeFallingState = Cast<UPlayerLedgeFallingState>(StateController->FindStateOfClass(LedgeFallingStateClass));
	KnockbackState = Cast<UPlayerKnockbackState>(StateController->FindStateOfClass(KnockbackStateClass));
	RollState = Cast<UPlayerRollState>(StateController->FindStateOfClass(RollStateClass));
	TM = &GetWorld()->GetTimerManager();
}

void UPlayerCombatState::EnterState()
{
	Super::EnterState();

	CharacterMaxSpeed = MainCharacter->MovementComponent->MaxWalkSpeed;
	MainCharacter->ToggleInteraction(false);
	MainCharacter->OnKnockbackRequested.AddDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnAttackRequested.AddDynamic(this, &ThisClass::AttackInputHandle);
	
	PreviousGravityScale = MainCharacter->MovementComponent->GravityScale;
	if (StateController->PreviousState->IsA(UPlayerMidAirState::StaticClass()))
	{
		if (StateController->PreviousState->IsA(UPlayerJumpState::StaticClass()) && MainCharacter->GetVelocity().Z >= 0.0f)
		{
			MainCharacter->MovementComponent->StopMovementImmediately();
			MainCharacter->ResetJumpState();
		}
		
		MainCharacter->MovementComponent->GravityScale = PreviousGravityScale * MidAirGravityWeight;
		CharacterController->OnMoveInput.AddDynamic(this, &ThisClass::HandleMidAirMovement);
		StateReady = false;
	}
	else
	{
		SetGroundedBehavior();
		if (StateController->PreviousState != IdleState)
		{
			FRotator TargetRotation = FRotationMatrix::MakeFromX(CharacterController->MovementDirection).Rotator();
			MainCharacter->SetActorRotation(TargetRotation);
		}
	}

	CanTick = true;
	StartAttack();
	
	MainCharacter->GetAbilitySystemComponent()->GenericGameplayEventCallbacks.FindOrAdd(ComboWindowOpenTag)
		   .AddUObject(this, &UPlayerCombatState::ComboWindowOpen);
	MainCharacter->GetAbilitySystemComponent()->GenericGameplayEventCallbacks.FindOrAdd(CommitAttackTag)
		   .AddUObject(this, &UPlayerCombatState::ConsumeBuffer);
	MainCharacter->GetAbilitySystemComponent()->GenericGameplayEventCallbacks.FindOrAdd(CommitAttackTag)
	       .AddUObject(this, &UPlayerCombatState::ResetCanRotate);	
	
	UE_LOG(LogTemp, Log, TEXT("PLAYER STATE: Combat State"));
}

void UPlayerCombatState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	if (!MainCharacter->MovementComponent->IsFalling())
	{
		MainCharacter->MovementComponent->StopMovementImmediately();
		CharacterController->OnMoveInput.RemoveDynamic(this, &ThisClass::HandleMidAirMovement);
		StateReady = true;
		SetGroundedBehavior();
		CanTick = false;
	}
}

void UPlayerCombatState::ExitState()
{
	Super::ExitState();
	
	//Reset hit bool
	IsHitSuccess = IsDeflect = false;

	MainCharacter->MovementComponent->MaxWalkSpeed = CharacterMaxSpeed;
	MainCharacter->MovementComponent->GravityScale = PreviousGravityScale;
	MainCharacter->OnKnockbackRequested.RemoveDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnMoveInput.RemoveDynamic(this, &ThisClass::HandleMidAirMovement);
	CharacterController->OnAttackRequested.RemoveDynamic(this, &ThisClass::AttackInputHandle);
	CharacterController->OnMoveInput.RemoveDynamic(this, &ThisClass::HandleCharacterRotation);
	CharacterController->OnRollRequested.RemoveDynamic(this, &ThisClass::HandleRollRequest);
	
	//input buffer
	MainCharacter->GetAbilitySystemComponent()->GenericGameplayEventCallbacks.FindOrAdd(ComboWindowOpenTag)
		   .RemoveAll(this);
	MainCharacter->GetAbilitySystemComponent()->GenericGameplayEventCallbacks.FindOrAdd(CommitAttackTag)
		   .RemoveAll(this);
	ValidInputBuffer = false;
	
	//cooldown removed
	StateReady = true;
}

void UPlayerCombatState::SetAttackReady()
{
	if (TM->GetTimerRemaining(AttackCooldownTimer) > 0)
		return;
	
	if (MainCharacter->MovementComponent->IsFalling())
		return;
	
	StateReady = true;
}

void UPlayerCombatState::StartAttack_Implementation() {}

void UPlayerCombatState::EndAttack() const
{
	
	if (StateController->PreviousState->IsA(UPlayerMidAirState::StaticClass()))
		StateTransition(LedgeFallingState);
	else
		StateTransition(IdleState);
}

void UPlayerCombatState::HandleMidAirMovement(FVector MovementDirection)
{
	const FVector ForwardMovementComponent = MovementDirection.ProjectOnTo(MainCharacter->GetActorForwardVector()) * ForwardMidAirControlFactor * MidAirControlWeight;
	const FVector HorizontalMovementComponent = (MovementDirection - ForwardMovementComponent) * HorizontalMidAirControlFactor * MidAirControlWeight;

	MainCharacter->AddMovementInput(ForwardMovementComponent + HorizontalMovementComponent);
}

void UPlayerCombatState::GoToKnockbackState()
{
	MainCharacter->StopAnimMontage();
	StateTransition(KnockbackState);
}

void UPlayerCombatState::AttackInputHandle()
{
	StartAttack();
	InputBufferTimeStamp = GetWorld()->GetTimeSeconds();
	ValidInputBuffer = true;
	
	UE_LOG(LogTemp, Warning, TEXT("InputBufferTimeStamp: %f"), InputBufferTimeStamp);
}

void UPlayerCombatState::HandleCharacterRotation(FVector MoveDirection)
{
	if (!CanRotate)
		return;

	MainCharacter->AddMovementInput(MoveDirection);
}

void UPlayerCombatState::HandleRollRequest()
{
	StateTransition(RollState);
}

void UPlayerCombatState::SetGroundedBehavior()
{
	CanRotate = true;
	MainCharacter->MovementComponent->GravityScale = PreviousGravityScale;
	MainCharacter->MovementComponent->StopMovementImmediately();
	MainCharacter->MovementComponent->MaxWalkSpeed = 0.0f;

	CharacterController->OnMoveInput.RemoveDynamic(this, &ThisClass::HandleCharacterRotation);
	CharacterController->OnMoveInput.AddDynamic(this, &ThisClass::HandleCharacterRotation);
	
	CharacterController->OnRollRequested.RemoveDynamic(this, &ThisClass::HandleRollRequest);
	CharacterController->OnRollRequested.AddDynamic(this, &ThisClass::HandleRollRequest);
}


void UPlayerCombatState::ComboWindowOpen(const FGameplayEventData* Payload)
{
	if (ValidInputBuffer && GetWorld()->GetTimeSeconds() - InputBufferTimeStamp <= InputBufferDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("Consume Buffer: %f"), InputBufferTimeStamp);
		ValidInputBuffer = false;
		StartAttack();
	}
}

void UPlayerCombatState::ConsumeBuffer(const FGameplayEventData* Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Consume Buffer: %f"), InputBufferTimeStamp);
	ValidInputBuffer = false;
}

void UPlayerCombatState::ResetCanRotate(const FGameplayEventData* Payload)
{
	CanRotate = true;
}


void UPlayerCombatState::Hit(const AActor* HittedActor, const UPrimitiveComponent* CollisionBox, FVector Location)
{
	CanRotate = false;
	IsHitSuccess = true;
	HitFX.SpawnEffect(this, Location);
	
	StartTimeDilation();
}



void UPlayerCombatState::Deflect(const AActor* HittedActor, const UPrimitiveComponent* CollisionBox, FVector Location)
{
	//Avoid unnecessary Deflection in case the first hit is a success
	if (IsHitSuccess)
		return;

	CharacterController->OnRollRequested.RemoveDynamic(this, &ThisClass::HandleRollRequest);
	
	UE_LOG(LogTemp, Warning, TEXT("Deflect"));
	CanRotate = false;
	IsDeflect = true;
	DeflectFX.SpawnEffect(this,Location);
	StartTimeDilation();

	//send gameplay event
	FGameplayEventData EventData;
	EventData.EventTag = DeflectEventTag;
	EventData.Instigator = MainCharacter;
	EventData.Target = MainCharacter;
	EventData.OptionalObject = this;
	EventData.ContextHandle = MainCharacter->GetAbilitySystemComponent()->MakeEffectContext();
	int ActivatedAbilities = MainCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(DeflectEventTag, &EventData);
}
void UPlayerCombatState::ResetTimeDilation()
{
	if (MainCharacter)
	{
		MainCharacter->CustomTimeDilation = 1.0f;
	}
}
void UPlayerCombatState::StartTimeDilation()
{
	MainCharacter->CustomTimeDilation = 0;
	//HitSTop Logic
	if (GetWorld()->GetTimerManager().IsTimerActive(TimeDilationResetTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeDilationResetTimer);
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		TimeDilationResetTimer,
		this,
		&UPlayerCombatState::ResetTimeDilation,
		HitStopTime,
		false
	);
}