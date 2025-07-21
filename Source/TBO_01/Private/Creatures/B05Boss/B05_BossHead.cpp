// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/B05Boss/B05_BossHead.h"

#include "AIController.h"
#include "Creatures/B05Boss/B05_BossHand.h"
#include "Creatures/B05Boss/BossAiControllor.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AB05_BossHead::AB05_BossHead()
{

	PrimaryActorTick.bCanEverTick = true;

	LeftEye = CreateDefaultSubobject<USkeletalMeshComponent>("LeftEye");
	RightEye = CreateDefaultSubobject<USkeletalMeshComponent>("RightEye");
	
	LeftEye->SetupAttachment(GetMesh(),TEXT("Eye_LXSocket"));
	RightEye->SetupAttachment(GetMesh(), TEXT("Eye_RXSocket"));
	
}



void AB05_BossHead::BeginPlay()
{
	Super::BeginPlay();

	if (LeftEye && GetMesh())
	{
		LeftEye->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, LeftEyeSocketName);
	}
	if (RightEye && GetMesh())
	{
		RightEye->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, RightEyeSocketName);
	}
	
}

void AB05_BossHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


ACreature* AB05_BossHead::SpawnDestructibleHand()
{
	check(DestructibleHandClass);
	check(DestructibleHandControllerClass);
	DestructibleHand = Cast<AB05_BossHand>(SpawnMinion(DestructibleHandSpawnPosition->GetTransform(),DestructibleHandClass,DestructibleHandControllerClass));
	DestructibleHand->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	return DestructibleHand.Get();
}

ACreature* AB05_BossHead::SpawnStepOnHand()
{
	check(StepOnHandClass);
	check(StepOnHandControllerClass);
	StepOnHand =  Cast<AB05_BossHand>(SpawnMinion(StepOnHandSpawnPosition->GetTransform(),StepOnHandClass,StepOnHandControllerClass));
	
	StepOnHand->GetPatrolSplineComponent_Implementation()->AssignedPatrolPath = StepOnHandPatrolPath.Get();
	StepOnHand->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	
	return StepOnHand.Get();
}

ACreature* AB05_BossHead::SpawnMinion(FTransform TransformPosition, TSubclassOf<ACreature> MinionClass,
                                      TSubclassOf<AAIController> MinionControllerClass)
{
	UWorld* World = GetWorld();
	check(MinionClass);
	check(MinionControllerClass);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACreature* SpawnedMinion = World->SpawnActor<ACreature>(MinionClass,TransformPosition.GetLocation(),TransformPosition.GetRotation().Rotator(),SpawnParams);

	AAIController* MinionController = World->SpawnActor<AAIController>(MinionControllerClass,TransformPosition.GetLocation(),TransformPosition.GetRotation().Rotator(),SpawnParams);

	MinionController->Possess(SpawnedMinion);
	return SpawnedMinion;
	
}

void AB05_BossHead::SetEyesActive()
{
	LeftEye->SetVisibility(true);
	RightEye->SetVisibility(true);
}


void AB05_BossHead::DestroyHands()
{
	if (StepOnHand.IsValid())
	{
		StepOnHand->Destroy();
	}
	if (DestructibleHand.IsValid())
	{
		DestructibleHand->OnDeath.Clear();
		DestructibleHand->Destroy();
	}
	
}

void AB05_BossHead::SetHandsToRest()
{
	StepOnHand->SetRestPoint(StepOnHandRestPoint->GetActorLocation());
	DestructibleHand->SetRestPoint(DestructibleHandRestPoint->GetActorLocation());
	StepOnHand->SetActiveStatus(false);
	DestructibleHand->SetActiveStatus(false);
}
