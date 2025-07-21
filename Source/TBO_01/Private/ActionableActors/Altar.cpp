// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/Altar.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Creatures/B04/B04_CageBird.h"
#include "Creatures/B04/B04_Controller.h"
#include "Kismet/GameplayStatics.h"

AAltar::AAltar()
{
	BoxTriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTriggerArea"));
	BoxTriggerArea->SetupAttachment(RootComponent);
	
	AltarMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMeshComponent"));
	AltarMeshComponent->SetupAttachment(RootComponent);

	HintVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HintVFX"));
	HintVFX->SetupAttachment(RootComponent);
	
	BirdSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdSkeletalMesh"));
	BirdSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BirdSkeletalMesh->SetVisibility(false);
	BirdSkeletalMesh->SetupAttachment(RootComponent);

	BirdPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BirdPosition"));
	BirdPosition->SetupAttachment(RootComponent);
}

void AAltar::SetActiveMeshStateVisual() const
{
	BirdSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BirdSkeletalMesh->SetVisibility(true);
}

void AAltar::OnAltarAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(BirdClass))
		return;
	
	const AB04_CageBird* Bird = Cast<AB04_CageBird>(OtherActor);
	BirdController = Cast<AB04_Controller>(Bird->GetController());
	BirdController->OnReachedAltar.RemoveDynamic(this, &ThisClass::HandleBirdReachedAltar);
	BirdController->OnReachedAltar.AddDynamic(this, &ThisClass::HandleBirdReachedAltar);
	
	BirdController->AssignAltarPosition(BirdPosition->GetComponentTransform(), false);
}

void AAltar::HandleBirdReachedAltar()
{
	if (BirdController)
	{
		BirdController->OnReachedAltar.RemoveDynamic(this, &ThisClass::HandleBirdReachedAltar);
	}
	
	UGameplayStatics::PlaySound2D(this, AltarSound);
	ActionActors();
}

void AAltar::BeginPlay()
{
	BoxTriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAltarAreaBeginOverlap);
	
	Super::BeginPlay();
}

void AAltar::ClearTriggerArea()
{
	BoxTriggerArea->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnAltarAreaBeginOverlap);
	BoxTriggerArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAltar::RestoreState_Implementation()
{
	SetActiveMeshStateVisual();
}

void AAltar::ActionActors(const bool InstantActivation)
{
	Super::ActionActors(InstantActivation);

	ClearTriggerArea();

	AltarMeshComponent->SetMaterial(0, ActiveAltarMaterial);
	
	if (InstantActivation)
		SetActiveMeshStateVisual();
}
