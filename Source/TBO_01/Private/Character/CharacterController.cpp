// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterController.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/MainCharacter.h"
#include "FSM/StateController.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Lifesystem/LifeSystemComponent.h"
#include "SaveSystem/SaveSubsystem.h"

void ACharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//	Find enhanced input component
	UEnhancedInputComponent * enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(!enhancedInputComponent)
		return;

	//	Bind Input callbacks
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveAction);
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ThisClass::HandleStartMovement);
	enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::HandleJumpAction);
	enhancedInputComponent->BindAction(SlowLocomotionAction, ETriggerEvent::Triggered, this, &ThisClass::HandleSlowLocomotion);
	enhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::HandleRollAction);
	enhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::HandleAttackAction);
	enhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInteractionAction);
	enhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMenuAction);
	enhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ThisClass::HandlePauseAction);

	// Bind to handle input device change
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInputDeviceChange);
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ThisClass::HandleInputDeviceChange);
	enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInputDeviceChange);
	enhancedInputComponent->BindAction(SlowLocomotionAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInputDeviceChange);
	enhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInputDeviceChange);
	enhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInputDeviceChange);
	enhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInputDeviceChange);
	enhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInputDeviceChange);
}

void ACharacterController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	PossessedMainCharacter = Cast<AMainCharacter>(PossessedPawn);
	PossessedMainCharacter->StateControllerComponent->Init();
	PossessedMainCharacter->ListenToInputDeviceChange();
	InputDeviceSubsystem = GetGameInstance()->GetEngine()->GetEngineSubsystem<UInputDeviceSubsystem>();
	ToggleControls(true);
	SetInputContextEnabled(PauseMappingContext, true);
	PossessedMainCharacter->OnInteractableInRange.AddDynamic(this, &ThisClass::ToggleInteractionInput);
	PossessedMainCharacter->GetLifeComponent()->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
}

void ACharacterController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedMainCharacter->GetLifeComponent()->OnDeath.RemoveDynamic(this, &ThisClass::HandleDeath);
	PossessedMainCharacter->OnInteractableInRange.RemoveDynamic(this, &ThisClass::ToggleInteractionInput);
	ToggleControls(false);
	SetInputContextEnabled(PauseMappingContext, false);
}

void ACharacterController::SetInputContextEnabled(UInputMappingContext* Context, bool bEnabled, int Priority) const
{
	//	Check valid context
	if(!Context)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid context to de/activate."));
		return;
	}

	//	Check valid input subsystem
	const auto EnhancedInput = GetInputSubsystem();
	if(!EnhancedInput)
		return;

	//	Add or remove the given context, based on the request
	if(bEnabled)
		EnhancedInput->AddMappingContext(Context, Priority);
	else
		EnhancedInput->RemoveMappingContext(Context);
}

void ACharacterController::HandleMoveAction(const FInputActionValue& Value)
{	
    FVector CamLoc;
    FRotator CamRot;
    PlayerCameraManager->GetCameraViewPoint(CamLoc, CamRot);

    FVector2D CurrentMovementVector = Value.Get<FVector2D>();

	FVector Forward;
	FVector Right;

    if (KeepCameraRotationDirection) 
    {
        if (RecentlyUsedDevice == EHardwareDevicePrimaryType::Gamepad)
        {
            MovementVector = CurrentMovementVector;

        	Forward = CamRot.Vector();
        	Right = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);
        }
        else
        {
            // If movement change
            if (CurrentMovementVector != MovementVector)
            {
                SavedCamRot = CamRot;
                MovementVector = CurrentMovementVector;
            }

        	Forward = SavedCamRot.Vector();
        	Right = FRotationMatrix(SavedCamRot).GetUnitAxis(EAxis::Y);
        }
    }
    else 
    {
        MovementVector = CurrentMovementVector;

    	Forward = CamRot.Vector();
    	Right = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);
    }

	Forward.Z = 0.f;
	Right.Z = 0.f;
	Right.Normalize();
	
	if (FMath::IsNearlyEqual(Forward.SquaredLength(), 0.0f))
		Forward = FVector::CrossProduct(Right, FVector::UpVector);
	
	Forward.Normalize();

	MovementDirection = Right * MovementVector.X + Forward * MovementVector.Y;
    MovementDirection.Normalize();
	
    if (RecentlyUsedDevice == EHardwareDevicePrimaryType::Gamepad)
    {
        SlowMovement = MovementVector.SquaredLength() <= SlowMoveAnalogThreshold * SlowMoveAnalogThreshold;
    }
    OnMoveInput.Broadcast(MovementDirection);
}

void ACharacterController::HandleStartMovement(const FInputActionValue& Value)
{
	FVector CamLoc;
	FRotator CamRot;
	PlayerCameraManager->GetCameraViewPoint(CamLoc, CamRot);
	MovementVector = Value.Get<FVector2D>();
	SavedCamRot = CamRot;
}

void ACharacterController::HandleJumpAction(const struct FInputActionValue& Value)
{
	OnJumpRequested.Broadcast();
}

void ACharacterController::HandleInputDeviceChange(const struct FInputActionValue& Value)
{
	const EHardwareDevicePrimaryType DetectedDevice = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(GetPlatformUserId()).PrimaryDeviceType;
	
	if (DetectedDevice != RecentlyUsedDevice)
	{
		RecentlyUsedDevice = DetectedDevice;
		SlowMovement = false;
		OnInputDeviceChange.Broadcast(DetectedDevice);
		
		UE_LOG(LogTemp, Log, TEXT("Using %s"), *UEnum::GetValueAsString(RecentlyUsedDevice));
	}
}

void ACharacterController::HandleSlowLocomotion(const struct FInputActionValue& Value)
{
	// If pressed is TRUE, if released is FALSE
	SlowMovement = Value.Get<bool>();
}

void ACharacterController::HandleRollAction(const struct FInputActionValue& Value)
{
	OnRollRequested.Broadcast();
}

void ACharacterController::HandleAttackAction(const struct FInputActionValue& Value)
{
	OnAttackRequested.Broadcast();
}

void ACharacterController::HandleInteractionAction(const struct FInputActionValue& Value)
{
	PossessedMainCharacter->TriggerInteraction();
}

void ACharacterController::HandleMenuAction(const FInputActionValue& Value)
{
	ToggleMenu();
}

void ACharacterController::HandlePauseAction(const struct FInputActionValue& Value)
{
	ToggleGamePause();
}

void ACharacterController::ToggleInteractionInput(bool Value, USceneComponent* InteractionPromptLocationComponent)
{
	SetInputContextEnabled(InteractionMappingContext, Value);
}

void ACharacterController::ToggleControls(bool Value)
{
	SetInputContextEnabled(LocomotionMappingContext, Value);
	SetInputContextEnabled(CombatMappingContext, Value);
	SetInputContextEnabled(MenuMappingContext, Value);
	SlowMovement = false;
}

void ACharacterController::ToggleInput(bool Value)
{
	SetInputContextEnabled(PauseMappingContext, Value);
	
	PossessedMainCharacter->OnInteractableInRange.RemoveDynamic(this, &ThisClass::ToggleInteractionInput);
	if (Value)
	{
		PossessedMainCharacter->OnInteractableInRange.AddDynamic(this, &ThisClass::ToggleInteractionInput);
	}
	else
	{
		ToggleInteractionInput(Value, nullptr);
	}
	
	ToggleControls(Value);
}

void ACharacterController::HandleDeath(USceneComponent* InstigatorComponent)
{
	ToggleControls(false);

	// remove all active effects
	PossessedMainCharacter->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(FGameplayTagContainer());
	
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	SaveSubsystem->CleanSaveGamePointer();

	ShowGameOver();
}

UEnhancedInputLocalPlayerSubsystem* ACharacterController::GetInputSubsystem() const
{
	//	Get local player
	const auto LocalPlayer = GetLocalPlayer();
	if(!LocalPlayer)
		return nullptr;

	//	Get enhanced input subsystem
	return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}
