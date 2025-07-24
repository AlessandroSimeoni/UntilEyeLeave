// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Camera/Interface/TargetCameraActor.h"
#include "Creatures/TeamAffiliationConfig.h"
#include "GameFramework/Character.h"
#include "Interface/HittableActor.h"
#include "Interface/MotionWarping.h"
#include "Lifesystem/LifeBridgeInterface.h"
#include "MainCharacter.generated.h"

class UDamageFlash;
enum class EHardwareDevicePrimaryType : uint8;
class UStateController;
class UGameplayAbility;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteraction, bool, InRange, USceneComponent*, InteractionPromptLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKnockbackRequested);

UCLASS()
class TBO_01_API AMainCharacter : public ACharacter, public ITargetCameraActor, public IAbilitySystemInterface,
								  public ILifeBridgeInterface, public IHittableActor, public IMotionWarping, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	FCollisionQueryParams CollisionParams;
	FHitResult HitResult;
	FHitResult InteractionHitResult;
	bool CanInteract = false;
	bool Invulnerable = false;
	FTimerHandle InvulnerabilityTimer;
	FTimerManager* TM;
	FVector LastGroundedLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
	float InteractionDetectionRange = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly,Category="GAS");
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Life Component")
	ULifeSystemComponent* LifeComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Hit Received", Meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float InvulnerabilityDuration = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	UMotionWarpingComponent* MotionWarpingComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Settings")
	TEnumAsByte<ECollisionChannel> InteractionChannel = ECC_GameTraceChannel3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grounded Location")
	FName IgnoreGroundedLocationTagName = "IgnoreGroundedLocation";
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="FX")
	TObjectPtr<UDamageFlash> DamageFlashComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Affiliaton")
	FTeamAffiliationConfig TeamAffiliationConfig;

	
	
	UFUNCTION()
	void CheckInteraction();
	UFUNCTION()
	void MakePlayerVulnerable();
	void GiveDefaultAbility();

	UFUNCTION(BlueprintCallable)
	void SetCoin(float CoinQuantity);

public:
	/**
	 * Switch character rotation rate based on the target device
	 * @param DeviceType The target device
	 */
	UFUNCTION()
	void SetRotationRateOfDevice(EHardwareDevicePrimaryType DeviceType);

	/**
	 * Ledge grab detection logic
	 * @param VerticalTraceUpwardOffset Vertical trace upward offset from actor location
	 * @param VerticalTraceForwardOffset Vertical trace forward offset from actor location
	 * @param VerticalTraceLength Vertical trace length
	 * @param VerticalTraceRadius Vertical trace radius
	 * @param HorizontalTraceUpwardOffset Horizontal trace upward offset from actor location
	 * @param HorizontalTraceLength Horizontal trace length
	 * @param HorizontalTraceRadius Horizontal trace radius
	 * @param PelvisFromLedgeMaxOffset max offset between pelvis bone and the edge of the target surface to grab
	 * @param HorizontalHitResult Out horizontal hit result
	 * @param VerticalHitResult Out Vertical hit result
	 * @return true if ledge grab is possible, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Ledge Grab Detection")
	bool CanLedgeGrab(float VerticalTraceUpwardOffset, float VerticalTraceForwardOffset, float VerticalTraceLength, float VerticalTraceRadius, float HorizontalTraceUpwardOffset, float HorizontalTraceLength, float HorizontalTraceRadius, float PelvisFromLedgeMaxOffset, FHitResult& HorizontalHitResult, FHitResult& VerticalHitResult);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ToggleInteraction(bool Value);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TriggerInteraction();
	UFUNCTION(BlueprintCallable, Category = "Knockback")
	void ApplyKnockback(AActor* InstigatorActor, float Intensity);
	UFUNCTION()
	virtual ULifeSystemComponent* GetLifeComponent() override;
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity = 1.f) override;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetInvulnerability(bool Value);
	void DOTDamage(float Value);
	UFUNCTION(BlueprintCallable, Category = "Grounded Location")
	FVector GetLastGroundedLocation();
	UFUNCTION()
	void ListenToInputDeviceChange();

	virtual FGenericTeamId GetGenericTeamId() const override;
	
	AMainCharacter();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Device Based Character Rotation Rate", Meta = (UIMin = 0.0f, UIMax = 3000.0f, ClampMin = 0.0f))
	float ControllerYawRotationRate = 1000.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Device Based Character Rotation Rate", Meta = (UIMin = 0.0f, UIMax = 1000.0f, ClampMin = 0.0f))
	float PCYawRotationRate = 350.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	FName LedgeGrabTag;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	bool DebugRays = false;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Combat", Meta = (UIMin = 1.0, ClampMin = 1.0))
	float CurrentWeaponLevel = 1.0f;
    UPROPERTY(BlueprintReadOnly)
    AGameCamera* GameCamera;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent = nullptr;

	bool WasInInteractionRange = false;
	UPROPERTY(BlueprintReadWrite)
	FInteraction OnInteractableInRange;
	FKnockbackRequested OnKnockbackRequested;
	UPROPERTY(BlueprintReadOnly)
	AActor* KnockbackInstigator;
	UPROPERTY(BlueprintReadOnly)
	float KnockbackIntensity = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStateController* StateControllerComponent = nullptr;

	virtual UMotionWarpingComponent* GetMotionWarpingComponent_Implementation() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void SetCamera_Implementation(AGameCamera* Camera) override;
};
