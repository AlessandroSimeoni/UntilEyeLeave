// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/GameCamera.h"

#include "ShaderPrintParameters.h"
#include "Camera/DistanceToTargetModifier.h"
#include "Camera/LenghtModifier.h"
#include "Camera/LockOnModifier.h"
#include "Camera/OcclusionMaterialHandler.h"
#include "Camera/OffsetToForwardModifier.h"
#include "Camera/RotationModifier.h"
#include "Camera/SplineModifier.h"
#include "Camera/Interface/TargetCameraActor.h"

// Sets default values
AGameCamera::AGameCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpingArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraComponent->PrimaryComponentTick.TickGroup = TG_PostPhysics;
	SpringArmComponent->PrimaryComponentTick.TickGroup = TG_PostPhysics;

	PrimaryActorTick.TickGroup = TG_PostPhysics;

	OcclusionMaterialHandler = CreateDefaultSubobject<UOcclusionMaterialHandler>(TEXT("OcclusionMaterialHandler"));
}

// Called when the game starts or when spawned
void AGameCamera::BeginPlay()
{
	Super::BeginPlay();
	DefaultOffset =  SpringArmComponent->TargetOffset;
	DefaultRotation = SpringArmComponent->GetDesiredRotation();
	DefaultArmLenght = SpringArmComponent->TargetArmLength;
	CurrentInterpSpeed = CameraInterpSpeed;
}

void AGameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//inverse for too keep safe on remove
	UpdateModifier(DeltaTime);
	if (!IsValid(AttachedActor))
	{
		return;
	}
	FVector TargetLocation = AttachedActor->GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, CurrentInterpSpeed);
	SetActorLocation(NewLocation);
	
	
}

void AGameCamera::UpdateModifier(float DeltaTime)
{
	for (int32 i = SpringArmModifiers.Num() - 1; i >= 0; --i)
	{
		TScriptInterface<ISpringArmModifier> Modifier = SpringArmModifiers[i];
		
		Modifier->Update(DeltaTime, SpringArmComponent);

		//remove useless modifier
		if (Modifier->IsDestructible())
		{
			UE_LOG(LogTemp, Warning, TEXT("Remove %d"),i);
			SpringArmModifiers.RemoveAt(i);
		}
	}
}

UCameraComponent* AGameCamera::GetCameraComponent()
{
	return CameraComponent;
}

void AGameCamera::Attach(AActor* Actor, bool ZeroLocation = true)
{
	if (!Actor)
		return;


	CurrentInterpSpeed = CameraInterpSpeed;
	
	AttachedActor = Actor;
	if (AttachedActor)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			false);
	SetActorLocation(AttachedActor->GetActorLocation(),false, nullptr, ETeleportType::None);
	//	AttachToActor(AttachedActor,AttachmentRules);
	}
	
	if (ZeroLocation)
		this->SetActorRelativeTransform(FTransform::Identity);

	ResetSpringArmToBegin();

	if (AttachedActor->Implements<UTargetCameraActor>())
	{
		CastChecked<ITargetCameraActor>(AttachedActor)->Execute_SetCamera(AttachedActor, this);
	}
	
	SetDefaultModifiers();
	OcclusionMaterialHandler->InitializeOcclusionMaterial(this);
}

void AGameCamera::Detach()
{
	

	SetAttachedActor(nullptr);
	CurrentInterpSpeed = 0;
	
	FDetachmentTransformRules DetachmentRules(
   EDetachmentRule::KeepWorld,   
   EDetachmentRule::KeepRelative, 
   EDetachmentRule::KeepRelative, 
   false
);
	SpringArmModifiers.Empty();
	//DetachFromActor(DetachmentRules);
}


void AGameCamera::ResetSpringArmToBegin()
{
	SpringArmComponent->SetRelativeRotation(DefaultRotation);
	SpringArmComponent->TargetOffset = DefaultOffset;
	SpringArmComponent->TargetArmLength = DefaultArmLenght;
}


//Modifiers

//not Used
UObject*  AGameCamera::AddRotationModifier(const float DeltaYaw, const float DeltaPitch,
	const float DeltaRoll, const float Duration)
{
	URotationModifier* Modifier = URotationModifier::Create(this,DeltaYaw,DeltaPitch,DeltaRoll,Duration, CameraComponent);
	SpringArmModifiers.Add(Modifier);
	return Modifier;
}


UObject* AGameCamera::AddLenghtModifier(float DeltaLenght, float Duration)
{
	ULenghtModifier* Modifier = ULenghtModifier::Create(this,DeltaLenght,Duration);
	SpringArmModifiers.Add(Modifier);
	return Modifier;
}
//



UObject* AGameCamera::AddSplineModifier(USplineComponent* Spline,const FVector& Start, const FVector& End, const bool UseStartEndPointDistance, const bool AdaptSpringArmLenght, const bool Dolly, const bool DollyAligned, const bool ForwardAligned, float RotationSpeed, float LenghtSpeed, float SplineMovementSpeed)
{
	if (!IsValid(SplineModifier))
		return nullptr;
	RotationModifier->IsCameraRotating = Dolly;
	SplineModifier->SetUp(Spline,Start,End,UseStartEndPointDistance,AdaptSpringArmLenght, Dolly, DollyAligned, ForwardAligned, RotationSpeed, LenghtSpeed, SplineMovementSpeed);
	if (SpringArmModifiers.Contains(SplineModifier))
	{
		SplineModifier->IsEnded = false;
	}
	else
	{
		SpringArmModifiers.Add(SplineModifier);
		SplineModifier->IsEnded = false;
	}
	return nullptr;
}

UObject* AGameCamera::AddDistanceToTargetModifier(AActor* TargetActor, UCurveFloat* Curve, float TargetDeltaArmLength)
{
	if (!IsValid(TargetActor) || !IsValid(Curve))
		return nullptr;

	if (!IsValid(DistanceToTargetModifier))
	{
		DistanceToTargetModifier = UDistanceToTargetModifier::Create(this, AttachedActor, TargetActor, Curve, TargetDeltaArmLength, LenghtModifier->GetTargetArmLenght(), LenghtModifier);
	}
	else
	{
		DistanceToTargetModifier->SetUp(TargetActor, Curve, TargetDeltaArmLength, LenghtModifier->GetTargetArmLenght());
	}

	if (SpringArmModifiers.Contains(DistanceToTargetModifier))
	{
		DistanceToTargetModifier->IsEnded = false;
	}
	else
	{
		SpringArmModifiers.Add(DistanceToTargetModifier);
		DistanceToTargetModifier->IsEnded = false;
	}

	return DistanceToTargetModifier;
}

void AGameCamera::RemoveSplineModifier(bool ResetRotation = true, bool ResetLenght = true)
{
	SplineModifier->IsEnded = true;
	RotationModifier->IsCameraRotating = false;
	if (ResetRotation)
	{
		SetRotationToDeafult();
	}
	if (ResetLenght)
	{
		SetLenghtToDefault();
	}
}

void AGameCamera::RemoveDistanceToTargetModifier(bool ResetLength = true)
{
	if (!IsValid(DistanceToTargetModifier)) 
		return;
	DistanceToTargetModifier->IsEnded = true;
	if (ResetLength)
	{
		SetLenghtToDefault();
	}
}

void AGameCamera::ChangeRotation(FRotator Rotation)
{
	RotationModifier->SetUp(Rotation,InterpSpeed);
}

void AGameCamera::ChangeLenght(float Lenght)
{
	LenghtModifier->SetUp(Lenght,InterpSpeed);
}
void AGameCamera::SetLenghtToDefault()
{
	ChangeLenght(DefaultArmLenght);
}
void AGameCamera::SetRotationToDeafult()
{
	ChangeRotation(DefaultRotation);
}


//Default Modifier

UOffsetHandlerModifier* AGameCamera::CreateDefaultOffsetHandlerModifier()
{
	//if (!IsValid(OffsetHandlerModifier))
	//{
		UOffsetHandlerModifier* Modifier = UOffsetHandlerModifier::Create(
			this, AttachedActor, LockOffset, ForwardOffset, InterpSpeed, MinLockDistance, MaxLockDistance);
		OffsetHandlerModifier = Modifier;
	//}
	/*
	else
	{
		OffsetHandlerModifier->SetStatus(true,false);
		OffsetHandlerModifier->LockOnModifier->SetTargetActor(nullptr);
	}
	*/
	SpringArmModifiers.Add(OffsetHandlerModifier);
	return OffsetHandlerModifier;
}
URotationModifier* AGameCamera::CreateDefaultRotationModifier()
{
	if (!IsValid(RotationModifier))
	{
		URotationModifier* Modifier =  URotationModifier::Create(this,DefaultRotation.Yaw,DefaultRotation.Pitch,DefaultRotation.Roll,InterpSpeed, CameraComponent);
		RotationModifier = Modifier;
	}
	else
	{
		RotationModifier->SetUp(DefaultRotation,InterpSpeed);
	}
	SpringArmModifiers.Add(RotationModifier);
	return RotationModifier;
}
ULenghtModifier* AGameCamera::CreateDefaultLenghtModifier()
{
	if (!IsValid(LenghtModifier))
	{
		ULenghtModifier* Modifier =  ULenghtModifier::Create(this,DefaultArmLenght,InterpSpeed);
		LenghtModifier = Modifier;
	}
	else
	{
		LenghtModifier->SetUp(DefaultArmLenght,InterpSpeed);
	}
	SpringArmModifiers.Add(LenghtModifier);
	return LenghtModifier;
}
USplineModifier* AGameCamera::CreateDefaultSplineModifier()
{
	USplineModifier* Modifier =  USplineModifier::Create(this,nullptr,RotationModifier,LenghtModifier);
	SplineModifier = Modifier;
	return Modifier;
}

UDistanceToTargetModifier* AGameCamera::CreateDefaultDistanceToTargetModifier()
{
	UDistanceToTargetModifier* Modifier = UDistanceToTargetModifier::Create(
		this, AttachedActor, nullptr, nullptr, 0.f, SpringArmComponent->TargetArmLength, LenghtModifier
	);
	DistanceToTargetModifier = Modifier;
	return DistanceToTargetModifier;
}


void AGameCamera::SetDefaultModifiers()
{
	CreateDefaultOffsetHandlerModifier();
	CreateDefaultRotationModifier();
	CreateDefaultLenghtModifier();
	CreateDefaultSplineModifier();
	CreateDefaultDistanceToTargetModifier();
}


//offset Handle Function

void AGameCamera::SetLockOnTarget(AActor* Target)
{
	if (OffsetHandlerModifier)
		OffsetHandlerModifier->LockOnModifier->SetTargetActor(Target);
}

bool AGameCamera::iSLockOnTargetSet()
{
	if (OffsetHandlerModifier)
		return !OffsetHandlerModifier->LockOnModifier->TargetIsNull();
	else
	{
		return false;
		//UE_LOG(LogTemp, Warning, TEXT("Offset Handle Modifier dont exist"));
	}
		
		
}

void AGameCamera::SetOffsetForwardTarget(AActor* Target)
{
	if (OffsetHandlerModifier)
		OffsetHandlerModifier->OffsetForwardModifier->SetTargetActor(Target);
}

void AGameCamera::SetOffsetEnabled(bool Enabled, bool ImmediateZeroOffset)
{
	if (OffsetHandlerModifier)
		OffsetHandlerModifier->SetStatus(Enabled,ImmediateZeroOffset);
}

void AGameCamera::SetManualOffset(const FVector& Offset,bool UseDefaultOffset)
{
	OffsetHandlerModifier->SetDefaultOffset(UseDefaultOffset,Offset);
}

//


void AGameCamera::SetAttachedActor(AActor* Actor)
{
	AttachedActor = Actor;
	SetLockOnTarget(Actor);
	SetOffsetForwardTarget(Actor);
}


void AGameCamera::RevertModifier(UObject* Modifier, bool Immediate)
{
	
	if (!IsValid(Modifier))
		return;
	if (Modifier->Implements<USpringArmModifier>())
	{
		Cast<ISpringArmModifier>(Modifier)->Revert(Immediate);
	}
}

void AGameCamera::RevertAllAppliedModifier(bool Immediate)
{
	SpringArmModifiers.Empty();
	SetDefaultModifiers();
	if (Immediate)
	{
		ResetSpringArmToBegin();
	}
	/*
	for (auto Modifier : SpringArmModifiers)
	{
		Modifier->Revert(Immediate);
	}
	if (Immediate)
		UpdateModifier(0);
		*/
}






