// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"


class AMainCharacter;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputDeviceSignature, EHardwareDevicePrimaryType, DetectedDevice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveInput, FVector, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRollRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackRequested);

UCLASS()
class TBO_01_API ACharacterController : public APlayerController
{
	GENERATED_BODY()
//fields
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Locomotion")
	TObjectPtr<class UInputMappingContext> LocomotionMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Locomotion")
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Locomotion")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Locomotion")
	TObjectPtr<UInputAction> SlowLocomotionAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Locomotion")
	TObjectPtr<UInputAction> RollAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Combat")
	TObjectPtr<UInputMappingContext> CombatMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Combat")
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Interaction")
	TObjectPtr<UInputMappingContext> InteractionMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Interaction")
	TObjectPtr<UInputAction> InteractionAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Menu")
	TObjectPtr<UInputMappingContext> MenuMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Menu")
	TObjectPtr<UInputAction> MenuAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Pause")
	TObjectPtr<UInputMappingContext> PauseMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping|Pause")
	TObjectPtr<UInputAction> PauseAction;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (UIMin = 0.0f, UIMax = 1.0f, ClampMin = 0.0f, ClampMax = 1.0f))
	float SlowMoveAnalogThreshold = 0.5f;

	UPROPERTY(BlueprintReadOnly)
	AMainCharacter* PossessedMainCharacter = nullptr;
	
	UPROPERTY()
	TObjectPtr<UInputDeviceSubsystem> InputDeviceSubsystem = nullptr;
	UPROPERTY()
	FRotator SavedCamRot;
	
	
// methods
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn * PossessedPawn) override;
	virtual void OnUnPossess() override;
	
	class UEnhancedInputLocalPlayerSubsystem * GetInputSubsystem() const;
	
	UFUNCTION()
	void HandleMoveAction(const struct FInputActionValue & Value);
	UFUNCTION()
	void HandleStartMovement(const FInputActionValue& Value);
	UFUNCTION()
	void HandleJumpAction(const struct FInputActionValue & Value);
	UFUNCTION()
	void HandleInputDeviceChange(const struct FInputActionValue & Value);
	UFUNCTION()
	void HandleSlowLocomotion(const struct FInputActionValue & Value);
	UFUNCTION()
	void HandleRollAction(const struct FInputActionValue & Value);
	UFUNCTION()
	void HandleAttackAction(const struct FInputActionValue & Value);
	UFUNCTION()
	void HandleInteractionAction(const struct FInputActionValue & Value);
	UFUNCTION()
	void HandleMenuAction(const struct FInputActionValue & Value);
	UFUNCTION()
	void HandlePauseAction(const struct FInputActionValue & Value);
	UFUNCTION()
	void ToggleInteractionInput(bool Value, USceneComponent* InteractionPromptLocationComponent);
	UFUNCTION()
	void HandleDeath(USceneComponent* InstigatorComponent);
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleMenu();
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleGamePause();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameOver();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	bool KeepCameraRotationDirection = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	bool CanTeleport;
	
	UPROPERTY(BlueprintAssignable)
	FInputDeviceSignature OnInputDeviceChange;
	EHardwareDevicePrimaryType RecentlyUsedDevice = EHardwareDevicePrimaryType::KeyboardAndMouse;
	FMoveInput OnMoveInput;
	FJumpRequested OnJumpRequested;
	FRollRequested OnRollRequested;
	FAttackRequested OnAttackRequested;
	FVector2D MovementVector;
	FVector MovementDirection;
	bool SlowMovement = false;

	UFUNCTION(BlueprintCallable)
	void ToggleControls(bool Value);
	UFUNCTION(BlueprintCallable)
	void ToggleInput(bool Value);
	void SetInputContextEnabled(UInputMappingContext * Context, bool bEnabled, int Priority = 1) const;
	UFUNCTION(BlueprintCallable)
	UInputMappingContext* GetLocomotionMappingContext() const {return LocomotionMappingContext;};
	UFUNCTION(BlueprintCallable)
	UInputMappingContext* GetCombatMappingContext() const {return CombatMappingContext;};
};
