// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/AOERing.h"

#include "Character/MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Creatures/Creature.h"


AAOERing::AAOERing()
{

	PrimaryActorTick.bCanEverTick = true;
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this,&AAOERing::OnBeginOverlap);
	CapsuleCollision->OnComponentEndOverlap.AddDynamic(this,&AAOERing::OnEndOverlap);

	
	RingPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring Plane"));
	RingPlane->SetupAttachment(CapsuleCollision);
	
	RingArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring Area"));
	RingArea->SetupAttachment(CapsuleCollision);
	
}


void AAOERing::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);  
	CapsuleCollision->SetCapsuleRadius(1);
	CapsuleCollision->SetCapsuleHalfHeight(1);
	
}


void AAOERing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentTime >= TimeToReachMax)
	{
		Destroy();
		return;
	}
	
	CurrentTime += DeltaTime;
	
	float Progress = FMath::Clamp(CurrentTime / TimeToReachMax, 0.0f, 1.0f);
	
	float CurrentRadius = FMath::Lerp(0.0f, MaxSize, Progress);
	
	
	CapsuleCollision->SetCapsuleRadius(CurrentRadius);
	
	SetXYScale(RingPlane, CurrentRadius * RingPlaneGrowthFactor, CurrentRadius * RingPlaneGrowthFactor);
	SetXYScale(RingArea, CurrentRadius * RingAreaGrowthFactor, CurrentRadius * RingAreaGrowthFactor);
}

void AAOERing::StartRingExpand(float InDestructionLevel, float InDamage, float InMaxSize, float InTimeToReachMax, float KnockabackIntensity)
{
	UE_LOG(LogTemp, Warning, TEXT("AAOERing::StartRingExpand CALLED"));
	Damage = InDamage;
	DestructionLevel = InDestructionLevel;
	MaxSize = InMaxSize;
	TimeToReachMax = InTimeToReachMax;
	CurrentTime = 0;
	RingKnockbackIntensity = KnockabackIntensity;
	
	CapsuleCollision->SetCapsuleRadius(1);
	//value to keep the capsule line
	CapsuleCollision->SetCapsuleHalfHeight(InMaxSize + 300);

	SetXYScale(RingArea,0,0);
	SetXYScale(RingPlane,0,0);
	
	
	GetWorld()->GetTimerManager().SetTimer(AOETickTimerHandle, this, &AAOERing::CheckDamageOnOverlappingActors, CheckInterval, true);

	
	SetActorTickEnabled(true);  
}


void AAOERing::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor->IsA(ACreature::StaticClass()) || !OtherActor->Implements<UHittableActor>())
	{
		return;
	}
	OverlappingActors.Add(OtherActor);
	CheckForDamage(OtherActor);
}

void AAOERing::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OverlappingActors.Contains(OtherActor) )
	{
		OverlappingActors.Remove(OtherActor);
	}	
}

void AAOERing::SetXYScale(UStaticMeshComponent* StaticMesh, float InScaleX, float InScaleY)
{
	FVector CurrentScaleRing = StaticMesh->GetRelativeScale3D(); 
	CurrentScaleRing.X = InScaleX;
	CurrentScaleRing.Y = InScaleY;
	StaticMesh->SetRelativeScale3D(CurrentScaleRing);
}

void AAOERing::CheckForDamage(AActor* OtherActor)
{
	FVector Origin;
	FVector Extent;
	OtherActor->GetActorBounds(true, Origin, Extent);

	float TopZ = Origin.Z + Extent.Z;

	FVector RingCenter = CapsuleCollision->GetComponentLocation();

	if (TopZ < RingCenter.Z)
	{
		return;
	}

	FVector FlatOffset = FVector(Origin.X, Origin.Y, 0.0f) - FVector(RingCenter.X, RingCenter.Y, 0.0f);
	float DistanceXY = FlatOffset.Size();

	bool bBeyondRadius = DistanceXY > CapsuleCollision->GetScaledCapsuleRadius() - RingCollisionRadius;
	bool bBelowHeight = Origin.Z - Extent.Z < RingCenter.Z + RingCollisionHeight;

	if (bBeyondRadius && bBelowHeight)
	{
		ApplyDamage(OtherActor);
	}
}

void AAOERing::ApplyDamage(AActor* OtherActor)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	TScriptInterface<IHittableActor> HittableInstance = OtherActor;
	HittableInstance->Execute_Hit(OtherActor, DestructionLevel, Damage, this, RingKnockbackIntensity);
}

void AAOERing::CheckDamageOnOverlappingActors()
{
	for (AActor* Actor : OverlappingActors)
	{
		if (IsValid(Actor))
		{
			CheckForDamage(Actor);
		}
	}
}
