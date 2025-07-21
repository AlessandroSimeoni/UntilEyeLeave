// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/SpawnPlatform.h"

#include "Components/BoxComponent.h"

ASpawnPlatform::ASpawnPlatform()
{
	PlatformMeshA = CreateDefaultSubobject<UStaticMeshComponent>("PlatformMeshA");
	PlatformMeshA->SetCollisionProfileName(TEXT("NoCollision"));
	PlatformMeshA->SetComponentTickEnabled(false);
	PlatformMeshA->SetupAttachment(RootComponent);

	PlatformMeshB = CreateDefaultSubobject<UStaticMeshComponent>("PlatformMeshB");
	PlatformMeshB->SetCollisionProfileName(TEXT("NoCollision"));
	PlatformMeshB->SetComponentTickEnabled(false);
	PlatformMeshB->SetupAttachment(RootComponent);

	PlatformMeshC = CreateDefaultSubobject<UStaticMeshComponent>("PlatformMeshC");
	PlatformMeshC->SetCollisionProfileName(TEXT("NoCollision"));
	PlatformMeshC->SetComponentTickEnabled(false);
	PlatformMeshC->SetupAttachment(RootComponent);

	PlatformMeshD = CreateDefaultSubobject<UStaticMeshComponent>("PlatformMeshD");
	PlatformMeshD->SetCollisionProfileName(TEXT("NoCollision"));
	PlatformMeshD->SetComponentTickEnabled(false);
	PlatformMeshD->SetupAttachment(RootComponent);

	MeshTargetLocationA = CreateDefaultSubobject<USceneComponent>(TEXT("MeshTargetLocationA"));
	MeshTargetLocationA->SetupAttachment(RootComponent);

	MeshTargetLocationB = CreateDefaultSubobject<USceneComponent>(TEXT("MeshTargetLocationB"));
	MeshTargetLocationB->SetupAttachment(RootComponent);

	MeshTargetLocationC = CreateDefaultSubobject<USceneComponent>(TEXT("MeshTargetLocationC"));
	MeshTargetLocationC->SetupAttachment(RootComponent);
	
	MeshTargetLocationD = CreateDefaultSubobject<USceneComponent>(TEXT("MeshTargetLocationD"));
	MeshTargetLocationD->SetupAttachment(RootComponent);

	PlatformCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PlatformCollision"));
	PlatformCollision->SetupAttachment(RootComponent);
}

void ASpawnPlatform::UpdateCollisionBox(bool On)
{
	PlatformCollision->SetCollisionProfileName(On ? TEXT("BlockAllDynamic") : TEXT("NoCollision"));
	PlatformCollision->SetCanEverAffectNavigation(On);
	PlatformCollision->RecreatePhysicsState();
}

void ASpawnPlatform::InstantStateChange(bool On)
{	
	CurrentlyOn = On;

	PlatformMeshA->SetRelativeLocation(CurrentlyOn ? MeshTargetLocationA->GetRelativeLocation() : StartingLocationMeshA);
	PlatformMeshA->SetRelativeRotation(CurrentlyOn ? MeshTargetLocationA->GetRelativeRotation() : StartingRotationMeshA);

	PlatformMeshB->SetRelativeLocation(CurrentlyOn ? MeshTargetLocationB->GetRelativeLocation() : StartingLocationMeshB);
	PlatformMeshB->SetRelativeRotation(CurrentlyOn ? MeshTargetLocationB->GetRelativeRotation() : StartingRotationMeshB);

	PlatformMeshC->SetRelativeLocation(CurrentlyOn ? MeshTargetLocationC->GetRelativeLocation() : StartingLocationMeshC);
	PlatformMeshC->SetRelativeRotation(CurrentlyOn ? MeshTargetLocationC->GetRelativeRotation() : StartingRotationMeshC);

	PlatformMeshD->SetRelativeLocation(CurrentlyOn ? MeshTargetLocationD->GetRelativeLocation() : StartingLocationMeshD);
	PlatformMeshD->SetRelativeRotation(CurrentlyOn ? MeshTargetLocationD->GetRelativeRotation() : StartingRotationMeshD);

	MeshADynMatInstance->SetScalarParameterValue(FName("CanFloat"), CurrentlyOn ? 0.0f : 1.0f);
	MeshBDynMatInstance->SetScalarParameterValue(FName("CanFloat"), CurrentlyOn ? 0.0f : 1.0f);
	MeshCDynMatInstance->SetScalarParameterValue(FName("CanFloat"), CurrentlyOn ? 0.0f : 1.0f);
	MeshDDynMatInstance->SetScalarParameterValue(FName("CanFloat"), CurrentlyOn ? 0.0f : 1.0f);
	
	UpdateCollisionBox(CurrentlyOn);
}

void ASpawnPlatform::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StartingLocationMeshA = PlatformMeshA->GetRelativeLocation();
	StartingLocationMeshB = PlatformMeshB->GetRelativeLocation();
	StartingLocationMeshC = PlatformMeshC->GetRelativeLocation();
	StartingLocationMeshD = PlatformMeshD->GetRelativeLocation();

	StartingRotationMeshA = PlatformMeshA->GetRelativeRotation();
	StartingRotationMeshB = PlatformMeshB->GetRelativeRotation();
	StartingRotationMeshC = PlatformMeshC->GetRelativeRotation();
	StartingRotationMeshD = PlatformMeshD->GetRelativeRotation();
	
	MeshADynMatInstance = PlatformMeshA->CreateAndSetMaterialInstanceDynamic(0);
	MeshBDynMatInstance = PlatformMeshB->CreateAndSetMaterialInstanceDynamic(0);
	MeshCDynMatInstance = PlatformMeshC->CreateAndSetMaterialInstanceDynamic(0);
	MeshDDynMatInstance = PlatformMeshD->CreateAndSetMaterialInstanceDynamic(0);
	
	InstantStateChange(IsOn);
}

void ASpawnPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	PlatformCollision->SetCollisionProfileName(IsOn ? TEXT("BlockAllDynamic") : TEXT("NoCollision"));
	PlatformCollision->SetCanEverAffectNavigation(IsOn);
}

void ASpawnPlatform::ActivateActor_Implementation(bool InstantActivation)
{
}

void ASpawnPlatform::DeActivateActor_Implementation(bool InstantDeActivation)
{
}
