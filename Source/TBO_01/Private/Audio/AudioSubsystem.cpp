// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/AudioSubsystem.h"

#include "Components/AudioComponent.h"

void UAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &UAudioSubsystem::OnLevelChangedFromParams);
}
void UAudioSubsystem::Deinitialize()
{
	FWorldDelegates::OnWorldInitializedActors.RemoveAll(this);
	Super::Deinitialize();
}

void UAudioSubsystem::SimulateLevelLoadForEditor()
{
#if WITH_EDITOR
	if (bHasInitializedAudio)
	{
		return;
	}

	bHasInitializedAudio = true;

	UWorld* World = GetWorld();
	if (World)
	{
		OnLevelChanged(World);
	}
#endif
}

void UAudioSubsystem::OnLevelChangedFromParams(const FActorsInitializedParams& Params)
{
	OnLevelChanged(Params.World);
}
void UAudioSubsystem::OnLevelChanged(UWorld* LoadedWorld)
{
	UE_LOG(LogTemp, Display, TEXT("Level changed"));
	if (!LoadedWorld || !LoadedWorld->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("AudioSubsystem: Ignoring non-game world (%s)"), *LoadedWorld->GetName());
		return;
	}

	
	if (MetaSoundManager.IsValid())
	{
		if (UAudioComponent* Old = MetaSoundManager.Get())
		{
			Old->DestroyComponent();
		}
		MetaSoundManager.Reset();
	}
	
	USoundBase* MetaSound = MetaSoundSourceAsset.LoadSynchronous();
	if (!MetaSound)
	{
		UE_LOG(LogTemp, Error, TEXT("AudioSubsystem: MetaSound asset not loaded."));
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = TEXT("AudioHost_MetaSound");
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* AudioHost = LoadedWorld->SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity, SpawnParams);
	if (!AudioHost)
	{
		UE_LOG(LogTemp, Error, TEXT("AudioSubsystem: cant create Actor"));
		return;
	}
	AudioHost->SetActorHiddenInGame(false);
	AudioHost->SetActorEnableCollision(false);
	
	UAudioComponent* NewAudioComponent = NewObject<UAudioComponent>(AudioHost);
	if (!NewAudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AudioSubsystem: Cant create compoennt"));
		return;
	}
	
	NewAudioComponent->bAutoActivate = false;
	NewAudioComponent->RegisterComponent();
	AudioHost->AddInstanceComponent(NewAudioComponent);

	NewAudioComponent->SetSound(MetaSound);
	NewAudioComponent->Play();

	MetaSoundManager = NewAudioComponent;

	UE_LOG(LogTemp, Log, TEXT("AudioSubsystem: MetaSound initialized %s"), *LoadedWorld->GetName());
	SpottedEnemies = 0;
}

void UAudioSubsystem::ActivateMetaSound()
{
	if (MetaSoundManager.IsValid() && MetaSoundManager->IsActive())
	{
		MetaSoundManager->Play();
	}
}

void UAudioSubsystem::ChangeMusic(ESFXType Music)
{
	if (MetaSoundManager.IsValid())
	{
		MetaSoundManager->SetIntParameter(NewTrackParameter, static_cast<int32>(Music));
		MetaSoundManager->SetTriggerParameter(TriggerMusicPlayParameter);
		
	}
}

void UAudioSubsystem::SetCutOff(float CutOffValue)
{
	if (MetaSoundManager.IsValid())
	{
		MetaSoundManager->SetFloatParameter(CutoffFrequencyParameter, CutOffValue);
	}
}

void UAudioSubsystem::AddSpottedCombatAudio()
{
	SpottedEnemies++;

	if (SpottedEnemies > 0)
	{
		ChangeMusic(ESFXType::SFX_Combat);
	}
}

void UAudioSubsystem::RemoveSpottedCombatAudio()
{
	SpottedEnemies = FMath::Max(0, SpottedEnemies-1);
	if (SpottedEnemies == 0)
	{
		ChangeMusic(ESFXType::SFX_Gameplay);
	}
}

void UAudioSubsystem::RemoveCompletelySpottedCombatAudio()
{
	SpottedEnemies = 0;
	ChangeMusic(ESFXType::SFX_Gameplay);
}
