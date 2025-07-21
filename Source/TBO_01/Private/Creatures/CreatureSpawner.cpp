// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/CreatureSpawner.h"

#include "Creatures/Creature.h"
#include "Interface/GameplayObjectState.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"
#include "SaveSystem/UniqueIDComponent.h"

ACreatureSpawner::ACreatureSpawner()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	UniqueIDComp = CreateDefaultSubobject<UUniqueIDComponent>(TEXT("UniqueID"));
	
	SpawnVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnVFX"));
	SpawnVFX->SetupAttachment(RootComponent);
	SpawnVFX->SetAutoActivate(false);
}

void ACreatureSpawner::BeginPlay()
{
	Super::BeginPlay();

	SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	
	// delay to make sure the connected objects settle their state
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(DelayTH, this, &ThisClass::InitialSetup, 0.2f, false);
}

void ACreatureSpawner::HandleCreatureDeath(AActor* TargetCreature,const AActor* DeathInstigator )
{	
	if (PendingDependantObjects())
	{
		//Start timer to respawn the creature
		FTimerManager& TM = GetWorld()->GetTimerManager();
		TM.SetTimer(TH, this, &ThisClass::RespawnCreature, SpawnDelay, false);
	}
	else
	{
		UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame();
		TBOSaveGame->AddInteractedActor(UniqueIDComp->UniqueID);
	}
}

void ACreatureSpawner::RespawnCreature()
{
	// vfx
	SpawnVFX->Activate();

	// spawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Creature = GetWorld()->SpawnActor<ACreature>(CreatureClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	Creature->OnDeath.AddDynamic(this, &ThisClass::HandleCreatureDeath);
}

void ACreatureSpawner::InitialSetup()
{
	// actor to TScriptInterface conversion
	for (AActor* Actor : DependantGameplayObjects)
	{
		if (Actor && Actor->GetClass()->ImplementsInterface(UGameplayObjectState::StaticClass()))
		{
			TScriptInterface<IGameplayObjectState> InterfaceRef;
			InterfaceRef.SetObject(Actor);
			InterfaceRef.SetInterface(Cast<IGameplayObjectState>(Actor));

			DependantGameplayObjectsConverted.Add(InterfaceRef);
		}
	}
	
	if (Creature)
	{
		if (PendingDependantObjects())
		{
			CreatureClass = Creature.GetClass();
			Creature->OnDeath.AddDynamic(this, &ThisClass::HandleCreatureDeath);
		}
		else
		{
			UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame();
			if (TBOSaveGame->FindInteractedActor(UniqueIDComp->UniqueID))
				Creature->Death(false);
			else
				Creature->OnDeath.AddDynamic(this, &ThisClass::HandleCreatureDeath);
		}
	}
}

bool ACreatureSpawner::PendingDependantObjects()
{
	for (const TScriptInterface<IGameplayObjectState> GameplayObject : DependantGameplayObjectsConverted)
	{
		if (!IsValid(GameplayObject.GetObject()))
			continue;
		
		if (!IGameplayObjectState::Execute_HasBeenActivated(GameplayObject.GetObject()))
			return true;
	}
	
	return false;
}
