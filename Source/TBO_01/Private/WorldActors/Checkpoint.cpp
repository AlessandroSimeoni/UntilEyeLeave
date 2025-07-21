// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/Checkpoint.h"

#include "Character/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Lifesystem/LifeSystemComponent.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"
#include "SaveSystem/UniqueIDComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SaveArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SaveArea"));
	SaveArea->SetupAttachment(RootComponent);
	SaveArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SaveArea->SetCollisionObjectType(ECC_WorldStatic);
	SaveArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	SaveArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SaveArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSaveAreaBeginOverlap);

	InteractionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionArea"));
	InteractionArea->SetupAttachment(RootComponent);
	InteractionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionArea->SetCollisionObjectType(ECC_WorldStatic);
	InteractionArea->SetCollisionResponseToAllChannels(ECR_Block);
	InteractionArea->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	InteractionArea->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);

	HealArea = CreateDefaultSubobject<UBoxComponent>(TEXT("HealArea"));
	HealArea->SetupAttachment(RootComponent);	
	HealArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HealArea->SetCollisionObjectType(ECC_WorldStatic);
	HealArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	HealArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HealArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHealAreaBeginOverlap);

	RespawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnLocation"));
	RespawnLocation->SetupAttachment(RootComponent);

	BusStopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BusStopMesh"));
	BusStopMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	BusStopMesh->SetupAttachment(RootComponent);

	StopSignMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StopSignMesh"));
	StopSignMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	StopSignMesh->SetupAttachment(RootComponent);

	UniqueIDComponent = CreateDefaultSubobject<UUniqueIDComponent>(TEXT("UniqueIDComponent"));

	InteractionPromptLocation = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionPromptLocation"));
	InteractionPromptLocation->SetupAttachment(RootComponent);
}

void ACheckpoint::OnSaveAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AMainCharacter>(OtherActor);
	if (!Character)
		return;
	
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
	SaveSubsystem->LoadGame()->UnlockTeleportPoint(UniqueIDComponent->UniqueID, RespawnLocation->GetComponentLocation());
	SaveGame->LastCheckpointLocation = RespawnLocation->GetComponentLocation();
	SaveSubsystem->SaveGame();
	OnCheckpointInteracted.Broadcast(this);
	
	AnimateStopSign();
}

void ACheckpoint::OnHealAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<AMainCharacter>(OtherActor))
		return;

	ULifeSystemComponent* CharacterLifeComponent;
	if (Character)
	{
		CharacterLifeComponent = Character->GetLifeComponent();
	}
	else
	{
		CharacterLifeComponent = Cast<AMainCharacter>(OtherActor)->GetLifeComponent();
	}
	
	CharacterLifeComponent->ReceiveHeal(CharacterLifeComponent->MaxHealth);
}

void ACheckpoint::Interact_Implementation() { }

USceneComponent* ACheckpoint::GetInteractionPromptSceneComponent_Implementation()
{
	return InteractionPromptLocation.Get();
}

UUniqueIDComponent* ACheckpoint::GetUniqueIDComponent_Implementation()
{
	return UniqueIDComponent;
}

FVector ACheckpoint::GetRespawnLocation()
{
	return RespawnLocation -> GetComponentLocation();
}

