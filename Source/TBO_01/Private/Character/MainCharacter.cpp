// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"

#include "AbilitySystemComponent.h"
#include "Character/CharacterController.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/Interactable.h"
#include "Lifesystem/LifeSystemComponent.h"
#include "MotionWarpingComponent.h"
#include "Character/GAS/PlayerAttributeSet.h"
#include "SceneComponent/DamageFlash.h"

AMainCharacter::AMainCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	StateControllerComponent = CreateDefaultSubobject<UStateController>("StateController");
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	MovementComponent = GetCharacterMovement();

	LifeComponent = CreateDefaultSubobject<ULifeSystemComponent>("LifeComponent");
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("Motion Warping");

	DamageFlashComponent = CreateDefaultSubobject<UDamageFlash>("DamageFlash");
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// by default set yaw rotation rate based on the PC one
	MovementComponent->RotationRate = FRotator(0, PCYawRotationRate, 0);
	
	CollisionParams.AddIgnoredActor(this);

	if (IsValid(AbilitySystemComponent))
		AbilitySystemComponent->InitAbilityActorInfo(this,this);

	GiveDefaultAbility();

	TM = &GetWorld()->GetTimerManager();

	DamageFlashComponent->SetFlashTargetSkeletalMesh(GetMesh());
}

void AMainCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CanInteract)
		CheckInteraction();

	const AActor* GroundActor = MovementComponent->CurrentFloor.HitResult.GetActor();
	if (MovementComponent->IsMovingOnGround() && GroundActor && !GroundActor->ActorHasTag(IgnoreGroundedLocationTagName))
		LastGroundedLocation = GetActorLocation();
}

void AMainCharacter::CheckInteraction()
{
	bool InRangeForInteraction = GetWorld()->LineTraceSingleByChannel(InteractionHitResult,
	                                                                        GetActorLocation(),
	                                                                        GetActorLocation() + GetActorForwardVector() * InteractionDetectionRange,
	                                                                        InteractionChannel, CollisionParams);

	if (InRangeForInteraction)
	{
		const AActor* HitActor = InteractionHitResult.GetActor();
		if (!HitActor || !HitActor->Implements<UInteractable>())
		{
			InRangeForInteraction = false;
		}
	}
	
	if (InRangeForInteraction && !WasInInteractionRange)
	{
		const TScriptInterface<IInteractable> InteractableInstance = InteractionHitResult.GetActor();
		USceneComponent* InteractionPromptLocation = InteractableInstance->Execute_GetInteractionPromptSceneComponent(InteractionHitResult.GetActor());
		OnInteractableInRange.Broadcast(true, InteractionPromptLocation);
	}
	else if (!InRangeForInteraction && WasInInteractionRange)
	{
		OnInteractableInRange.Broadcast(false, nullptr);
	}
		
	WasInInteractionRange = InRangeForInteraction;
}

void AMainCharacter::SetRotationRateOfDevice(EHardwareDevicePrimaryType DeviceType)
{
	switch (DeviceType)
	{
		case EHardwareDevicePrimaryType::KeyboardAndMouse:
			MovementComponent->RotationRate = FRotator(0, PCYawRotationRate, 0);
			break;
		default:
			MovementComponent->RotationRate = FRotator(0, ControllerYawRotationRate, 0);
	}
}

bool AMainCharacter::CanLedgeGrab(float VerticalTraceUpwardOffset, float VerticalTraceForwardOffset, float VerticalTraceLength, float VerticalTraceRadius,
	float HorizontalTraceUpwardOffset, float HorizontalTraceLength, float HorizontalTraceRadius, float PelvisFromLedgeMaxOffset, FHitResult& HorizontalHitResult, FHitResult& VerticalHitResult)
{
	FVector	TraceStartPosition = GetActorLocation() + GetActorUpVector()*HorizontalTraceUpwardOffset;
	FVector	TraceEndPosition = TraceStartPosition + GetActorForwardVector()*HorizontalTraceLength;

	if (DebugRays)
		DrawDebugCylinder(GetWorld(), TraceStartPosition, TraceEndPosition, HorizontalTraceRadius, 12, FColor::Green, false, 0.05f);

	// first the horizontal trace
	if (GetWorld()->SweepSingleByChannel(HitResult, TraceStartPosition, TraceEndPosition,FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(HorizontalTraceRadius), CollisionParams))
	{
		HorizontalHitResult = HitResult;
		
		// then the vertical one
		TraceStartPosition = GetActorLocation() + GetActorUpVector()*VerticalTraceUpwardOffset + GetActorForwardVector()*VerticalTraceForwardOffset;
		TraceEndPosition = TraceStartPosition - GetActorUpVector()*VerticalTraceLength;

		if (DebugRays)
		{
			DrawDebugCylinder(GetWorld(), TraceStartPosition, TraceEndPosition, VerticalTraceRadius, 12, FColor::Green, false, 0.05f);
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 15.0f, FColor::Red, false, 1.0f);
		}

		if (HitResult.GetActor() && !HitResult.GetActor()->ActorHasTag(LedgeGrabTag))
			return false;
	
		if (GetWorld()->SweepSingleByChannel(HitResult, TraceStartPosition, TraceEndPosition,FQuat::Identity, ECC_Visibility,FCollisionShape::MakeSphere(VerticalTraceRadius), CollisionParams))
		{
			if (DebugRays)
				DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 15.0f, FColor::Red, false, 1.0f);
			
			const FVector PelvisLocation = GetMesh()->GetBoneLocation("pelvis");
			const float ZOffsetFromLedge = PelvisLocation.Z - HitResult.ImpactPoint.Z;

			if (ZOffsetFromLedge >= -PelvisFromLedgeMaxOffset)
			{
				VerticalHitResult = HitResult;
				return true;
			}
			
			return false;
		}
	}
	
	return false;
}

void AMainCharacter::ToggleInteraction(bool Value)
{
	CanInteract = Value;
	if (!CanInteract && WasInInteractionRange)
	{
		OnInteractableInRange.Broadcast(false, nullptr);
		WasInInteractionRange = false;
	}
}

void AMainCharacter::TriggerInteraction()
{
	TScriptInterface<IInteractable> InteractableInstance = InteractionHitResult.GetActor();
	InteractableInstance->Execute_Interact(InteractionHitResult.GetActor());
}

void AMainCharacter::ApplyKnockback(AActor* InstigatorActor, float Intensity)
{
	KnockbackIntensity = Intensity;
	KnockbackInstigator = InstigatorActor;
	OnKnockbackRequested.Broadcast();
}

ULifeSystemComponent* AMainCharacter::GetLifeComponent()
{
	return LifeComponent;
}

bool AMainCharacter::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator,float Intensity)
{
	if (Invulnerable || LifeComponent->GetCurrentHealth() <= 0.0f)
	{
		return false;
	}

	LifeComponent->ReceiveDamage(DamageValue);
	DamageFlashComponent->FlashDamage();
	if (AttackInstigator)
	{
		ApplyKnockback(AttackInstigator,Intensity);
	}

	Invulnerable = true;
	TM->SetTimer(InvulnerabilityTimer, this, &ThisClass::MakePlayerVulnerable, InvulnerabilityDuration, false);
	
	return true;
}

void AMainCharacter::SetInvulnerability(const bool Value)
{
	if (!Value && TM->IsTimerActive(InvulnerabilityTimer))
		return;
	
	Invulnerable = Value;
}

void AMainCharacter::DOTDamage(float Value)
{
	if (LifeComponent->GetCurrentHealth() <= 0.0f)
		return;

	LifeComponent->ReceiveDamage(Value);
}

FVector AMainCharacter::GetLastGroundedLocation()
{
	return LastGroundedLocation;
}

void AMainCharacter::ListenToInputDeviceChange()
{
	// subscribe to input device change event
	Cast<ACharacterController>(GetController())->OnInputDeviceChange.AddDynamic(this, &ThisClass::SetRotationRateOfDevice);
}

FGenericTeamId AMainCharacter::GetGenericTeamId() const
{
	return TeamAffiliationConfig.GetTeam();
}

void AMainCharacter::MakePlayerVulnerable()
{
	Invulnerable = false;
	TM->ClearTimer(InvulnerabilityTimer);
	UE_LOG(LogTemp, Warning, TEXT("Invulnerability timer over"));
}

void AMainCharacter::SetCamera_Implementation(AGameCamera* Camera)
{
	GameCamera = Camera;
	Cast<ACharacterController>(GetController())->SetViewTargetWithBlend(Camera);
}

UMotionWarpingComponent* AMainCharacter::GetMotionWarpingComponent_Implementation()
{
	return MotionWarpingComponent;
}

UAbilitySystemComponent* AMainCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMainCharacter::GiveDefaultAbility()
{
	if (!IsValid(AbilitySystemComponent))return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbility)
	{
		//Create a representation of the ability
		const FGameplayAbilitySpec AbilitySpec(AbilityClass);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void AMainCharacter::SetCoin(float CoinQuantity)
{
	if (!AbilitySystemComponent)
	{
		
		return;
	}

	FGameplayAttribute CoinAttribute = UPlayerAttributeSet::GetCoinAttribute();

	if (CoinAttribute.IsValid())
	{
		AbilitySystemComponent->SetNumericAttributeBase(CoinAttribute, CoinQuantity);
		
	}
	
}
