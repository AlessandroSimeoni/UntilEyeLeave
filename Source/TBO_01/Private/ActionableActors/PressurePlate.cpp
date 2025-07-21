// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/PressurePlate.h"

#include "ActionableActors/ActionableActor.h"
#include "ActionableActors/PressurePlateAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Creatures/Creature.h"
#include "Creatures/CreatureAiController.h"
#include "Creatures/CreatureEnumHelper.h"
#include "Creatures/ShooterCreature.h"

APressurePlate::APressurePlate()
{
	BoxTriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTriggerArea"));
	BoxTriggerArea->SetupAttachment(RootComponent);

	PressurePlateSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PressurePlateSkeletalMesh"));
	PressurePlateSkeletalMesh->SetupAttachment(RootComponent);
}

void APressurePlate::OnPressurePlateBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;
	
	if (OtherActor->IsA(DestructibleGianguscioClass))
	{
		if (HasBrokenShell(OtherActor))
			return;

		const AShooterCreature* Creature = Cast<AShooterCreature>(OtherActor);
		ACreatureAiController* CreatureController = Cast<ACreatureAiController>(Creature->GetController());
		CreatureController->OnActorDamage.RemoveDynamic(this, &ThisClass::HandleBrokenShellDamage);
		CreatureController->OnActorDamage.AddDynamic(this, &ThisClass::HandleBrokenShellDamage);
	}
	
	ACreature* TargetCreature = Cast<ACreature>(OtherActor);
	TargetCreature->OnDeath.RemoveDynamic(this, &ThisClass::HandleCreatureEndOverlap);
	TargetCreature->OnDeath.AddDynamic(this, &ThisClass::HandleCreatureEndOverlap);
	
	if(CurrentOverlappedActors == 0)
		ActionActors();

	CurrentOverlappedActors++;
}

void APressurePlate::HandleCreatureEndOverlap(AActor* Actor, const AActor* DeathInstigator)
{
	ACreature* TargetCreature = Cast<ACreature>(Actor);
	TargetCreature->OnDeath.RemoveDynamic(this, &ThisClass::HandleCreatureEndOverlap);
	
	CurrentOverlappedActors--;

	if(CurrentOverlappedActors == 0)
		DeActivateActors();
}

void APressurePlate::OnPressurePlateEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
		return;

	if (OtherActor->IsA(DestructibleGianguscioClass) && HasBrokenShell(OtherActor))
		return;
	
	HandleCreatureEndOverlap(OtherActor);
}

void APressurePlate::HandleBrokenShellDamage(AActor* AttackerActor, AActor* Receiver, EDamageAction DamageType,
	float DamageValue)
{
	if (DamageType != EDamageAction::DestructionDamage)
		return;
	
	const AShooterCreature* Creature = Cast<AShooterCreature>(Receiver);
	ACreatureAiController* CreatureController = Cast<ACreatureAiController>(Creature->GetController());
	CreatureController->OnActorDamage.RemoveDynamic(this, &ThisClass::HandleBrokenShellDamage);

	HandleCreatureEndOverlap(Receiver);
}

bool APressurePlate::HasBrokenShell(const UAbilitySystemComponent* TargetASC)
{
	const FGameplayTag BrokenShellTag = FGameplayTag::RequestGameplayTag(FName("Status.BrokenShell"));
	if (TargetASC && TargetASC->HasMatchingGameplayTag(BrokenShellTag))
		return true;

	return false;
}

bool APressurePlate::HasBrokenShell(const AActor* TargetActor)
{
	const AShooterCreature* Creature = Cast<AShooterCreature>(TargetActor);
	const UAbilitySystemComponent* ASC = Creature->GetAbilitySystemComponent();
	return HasBrokenShell(ASC);
}

void APressurePlate::BeginPlay()
{
	PressurePlateAnimInstance = Cast<UPressurePlateAnimInstance>(PressurePlateSkeletalMesh->GetAnimInstance());
	BoxTriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPressurePlateBeginOverlap);
	BoxTriggerArea->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnPressurePlateEndOverlap);

	Super::BeginPlay();
}

void APressurePlate::ActionActors(const bool InstantActivation)
{
	// pressure plate doesn't save its state
	
	if(InstantActivation)
		return;

	for (const TObjectPtr<AActionableActor> Actionable : ConnectedActionableActor)
		if (Actionable)
			Actionable->RequestActivation(InstantActivation);
	
	Activated = true;
	PressurePlateAnimInstance->IsActive = Activated;
	MoveGroundCollisions(true);
}

void APressurePlate::DeActivateActors(const bool InstantDeActivation)
{
	// pressure plate doesn't save its state
	
	if(InstantDeActivation)
		return;
	
	for (const TObjectPtr<AActionableActor> Actionable : ConnectedActionableActor)
		if (Actionable)
			Actionable->RequestDeActivation(InstantDeActivation);

	Activated = false;
	PressurePlateAnimInstance->Montage_Play(DeactivatePressureAM);
	PressurePlateAnimInstance->IsActive = Activated;
	MoveGroundCollisions(false);
}
