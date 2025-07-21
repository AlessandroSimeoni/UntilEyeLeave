// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Creature.h"
#include "Perception/AISense_Touch.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CapsuleComponent.h"
#include "Creatures/CreatureAiController.h"
#include "Creatures/GAS/BaseCreatureAttributeSet.h"
#include "Creatures/Senses/ComplexCreatureSight.h"
#include "Currency/CoinContainer.h"
#include "Utility/EffectData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "SceneComponent/DamageFlash.h"
#include "Utility/OptimizationSubsystem.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Death, "FeedBack.Death.InstantDeath")
// Sets default values
ACreature::ACreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	PatrolSplineComponent = CreateDefaultSubobject<UPatrolSplineComponent>("PatrolSplineComponent");
	EyesHandlerActorComponent = CreateDefaultSubobject<UEyesHandlerActorComponent>("EyesHandlerActorComponent");
	DamageFlashComponent = CreateDefaultSubobject<UDamageFlash>("DamageFlash");
	CoinContainer = CreateDefaultSubobject<UCoinContainer>("CoinContainer");
	CoinContainer->SetupAttachment(RootComponent);
}



// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	DamageFlashComponent->SetFlashTargetSkeletalMesh(GetMesh());
	StartPosition = GetActorLocation();
	//actor that is the owner of the ability
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		BasicsAttributesSet = AbilitySystemComponent->GetSet<UBaseCreatureAttributeSet>();
		GetComponents<UEyeSceneComponent>(Eyes);

		if (!Eyes.IsEmpty())
		{
			EyesHandlerActorComponent->SetUpEyes(Eyes, BasicsAttributesSet->GetEyesHitPoint(),BasicsAttributesSet->GetEyeLevel(),BasicsAttributesSet->GetEyeTransitionTime(),GetMesh());
			UE_LOG(LogTemp, Warning, TEXT("Set Up eye"));
		}
		
		AbilitySystemComponent->SetNumericAttributeBase(UBaseCreatureAttributeSet::GetHealthAttribute(),FMath::Max(1,Eyes.Num()));
		
		GetCharacterMovement()->MaxWalkSpeed = BasicsAttributesSet->GetSpeed();
		GetCharacterMovement()->MaxFlySpeed = BasicsAttributesSet->GetSpeed();
	}

	if (EyesHandlerActorComponent)
	{
		EyesHandlerActorComponent->OnEyesHit.AddDynamic(this, &ACreature::OnEyeHit);
		EyesHandlerActorComponent->OnEyeDeath.AddDynamic(this, &ACreature::OnEyeDeath);
		EyesHandlerActorComponent->OnAllEyesDeath.AddDynamic(this, &ACreature::AllEyesDeath);
		
	}
	
	GiveDefaultAbility();
	
	Super::BeginPlay();
	if (bAddTickAggregator)
	{
		if (UOptimizationSubsystem* Opt =  UOptimizationSubsystem::Get(GetWorld()))
		{
			Opt->AddCreature(this);
		}
	}
}

void ACreature::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UOptimizationSubsystem* Opt =  UOptimizationSubsystem::Get(GetWorld()))
	{
		Opt->RemoveCreature(this);
	}
	Super::EndPlay(EndPlayReason);
}

void ACreature::SetOptimizationTick(bool bIsInRange)
{
	
	if (bIsInRange == bOptimizationTickEnabled)
	{
		return;
	}
	bOptimizationTickEnabled = bIsInRange;

	GetCharacterMovement()->SetComponentTickEnabled(bIsInRange);
	GetMesh()->SetComponentTickEnabled(bIsInRange);

	if (!CreatureAiController.IsValid())
	{
		return;
	}
	CreatureAiController->SetActorTickEnabled(bIsInRange);

	if (UComplexCreatureSight* ComplexCreatureSight = CreatureAiController->ComplexCreatureSightComponent )
	{
		ComplexCreatureSight->SetComponentTickEnabled(bIsInRange);
	}
	
	if (UPathFollowingComponent* PathFollowingComponent = CreatureAiController->GetPathFollowingComponent() )
	{
		PathFollowingComponent->SetComponentTickEnabled(bIsInRange);
	}
	if (CreatureAiController->BrainComponent)
	{
		CreatureAiController->BrainComponent->SetComponentTickEnabled(bIsInRange);
	}
	
}

// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AlignToGround(DeltaTime);
}


// Called to bind functionality to input
void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/////////////////////////////Ability System

UAbilitySystemComponent* ACreature::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



void ACreature::GiveDefaultAbility()
{
	if (!IsValid(AbilitySystemComponent))return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbility)
	{
		//Create a rappresentation of the ability
		const FGameplayAbilitySpec AbilitySpec(AbilityClass);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}


/////////////////////////////Hit


void ACreature::NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, OtherActor, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (!OtherActor) return;
	UAISense_Touch::ReportTouchEvent(GetWorld(),this,OtherActor,HitLocation);
	
}



/////////////////////////////Patrol Spline


UPatrolSplineComponent* ACreature::GetPatrolSplineComponent_Implementation()
{
	return PatrolSplineComponent;
}
/////////////////////////////Eye
UEyesHandlerActorComponent* ACreature::GetEyesHandlerActorComponent_Implementation()
{
	return EyesHandlerActorComponent;
}

/////////////////////////////Navigation
void ACreature::SetEffectNavigation(bool CanEffect)
{
	GetCapsuleComponent()->SetCanEverAffectNavigation(CanEffect);
}

/////////////////////////////Death
void ACreature::AllEyesDeath(UEyeSceneComponent* Eye, const AActor* ActorInstigator)
{
	OnAllEyesDeath(Eye,ActorInstigator);
	Killer = ActorInstigator;
}
void ACreature::Death(bool PlayFx)
{
	
	OnDeath.Broadcast(this,Killer);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
		AbilitySystemComponent->ClearAllAbilities();
	}
	FTimerHandle TimerHandle;
	if (PlayFx)
	{
		DeathFX.SpawnEffect(GetWorld(),GetActorLocation());
	}

	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 500.f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bHit && Hit.bBlockingHit)
	{
		
		CoinContainer->SpawnCoin(Killer ? Killer->GetActorLocation() : FVector::ZeroVector);
	}
	else
	{
		CoinContainer->SpawnCoin(Killer ? Killer->GetActorLocation() : FVector::ZeroVector, StartPosition);
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACreature::DelayedDestroy, 0.1f, false);
}

void ACreature::InstantDeath()
{
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("Death Ability"));
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_Death),true);
	}
}

void ACreature::Spawn()
{
	SpawnFX.SpawnEffect(GetWorld(),GetActorLocation());
}


void ACreature::DelayedDestroy()
{
	Destroy(); 
}

void ACreature::SwitchMovementMode(bool bUseDesiredRotationMode)
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		if (bUseDesiredRotationMode)
		{
			MovementComp->bUseControllerDesiredRotation = true;
			MovementComp->bOrientRotationToMovement = false;
		}
		else
		{
			MovementComp->bUseControllerDesiredRotation = false;
			MovementComp->bOrientRotationToMovement = true;
		}
	}
}

void ACreature::AlignToGround(float DeltaTime)
{
	if (!IsAlignedToGround)
		return;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		FRotator CurrentRot = GetMesh()->GetComponentRotation();
		FRotator TargetRotator = CurrentRot;

		if (MoveComp->IsFalling())
		{
			
			TargetRotator.Pitch = 0.f;
			TargetRotator.Roll = 0.f;
		}
		else if (MoveComp->CurrentFloor.bBlockingHit)
		{
			const FVector FloorNormal = MoveComp->CurrentFloor.HitResult.ImpactNormal;
			const float CurrentYaw = CurrentRot.Yaw;

		
			if (!FloorNormal.Equals(LastValidFloorNormal) || !FMath::IsNearlyEqual(CurrentYaw, LastYaw, 0.01f))
			{
				FVector RightVector = GetMesh()->GetRightVector();
				FVector ForwardVector = GetMesh()->GetForwardVector();

				FRotator NewRotYZ = FRotationMatrix::MakeFromYZ(RightVector, FloorNormal).Rotator();
				FRotator NewRotXZ = FRotationMatrix::MakeFromXZ(ForwardVector, FloorNormal).Rotator();

				CachedTargetRotation = FRotator(NewRotYZ.Pitch, CurrentYaw, NewRotXZ.Roll);

				
				LastValidFloorNormal = FloorNormal;
				LastYaw = CurrentYaw;
			}

			TargetRotator = CachedTargetRotation;
		}

	
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRotator, DeltaTime, 5.0f);
		GetMesh()->SetWorldRotation(NewRot);
	}
}

void ACreature::Teleport(const FVector& Position, EMovementMode Mode)
{
	TeleportTo(Position,GetActorRotation(),false,true);
	GetCharacterMovement()->SetMovementMode(Mode);
}