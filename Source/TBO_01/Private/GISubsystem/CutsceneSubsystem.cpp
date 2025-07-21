// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem/CutsceneSubsystem.h"

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Blueprint/UserWidget.h"
#include "GISubsystem/CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "UI/SkipCutsceneWidget.h"
#include "CommonUI/Public/Widgets/CommonActivatableWidgetContainer.h"

struct FLatentAction_PlayCutscene : FPendingLatentAction
{
	bool bIsFinished = false;

	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

	FLatentAction_PlayCutscene(const FLatentActionInfo& LatentInfo)
		: ExecutionFunction(LatentInfo.ExecutionFunction),
		  OutputLink(LatentInfo.Linkage),
		  CallbackTarget(LatentInfo.CallbackTarget)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.FinishAndTriggerIf(bIsFinished, ExecutionFunction, OutputLink, CallbackTarget);
	}
};

void UCutsceneSubsystem::PlayCutscene(UObject* WorldContext, FLatentActionInfo LatentInfo,
                                      TSoftObjectPtr<UWorld> CutsceneSublevel,
                                      TSoftObjectPtr<ULevelSequence> CutsceneLevelSequence)
{
	World = GEngine->GetWorldFromContextObjectChecked(WorldContext);
	if (!Sequence)
	{
		// load sequence if not preloaded
		Sequence = CutsceneLevelSequence.LoadSynchronous();
	}

	if (!World  || !Sequence)
	{
		return;
	}

	const FString LevelName = CutsceneSublevel.GetAssetName();
	CutsceneSublevelName = *LevelName;

	CurrentLatentUUID = LatentInfo.UUID;
	CutsceneLatentCallbackTarget = LatentInfo.CallbackTarget.Get();
	FLatentActionManager& LatentManager = World->GetLatentActionManager();
	if (!LatentManager.FindExistingAction<FLatentAction_PlayCutscene>(CutsceneLatentCallbackTarget, LatentInfo.UUID))
	{
		LatentManager.AddNewAction(CutsceneLatentCallbackTarget, LatentInfo.UUID, new FLatentAction_PlayCutscene(LatentInfo));
	}
	
	ULevelStreaming* StreamedLevel = UGameplayStatics::GetStreamingLevel(this, CutsceneSublevelName);
	if (StreamedLevel && StreamedLevel->IsLevelLoaded())
	{
		// if level has been preloaded with PreLoadCutsceneSublevel method than make it visible
		StreamedLevel->SetShouldBeVisible(true);
	}
	else
	{
		// Loads sublevel in streaming (async, latent)
		UGameplayStatics::LoadStreamLevel(WorldContext, CutsceneSublevelName, true, false, FLatentActionInfo());
	}
	
	// At the end of the loading, continue the execution
	TimerManager = &World->GetTimerManager();
	TimerManager->SetTimer(TimerHandle, this, &ThisClass::TryPlayCutscene, IsSublevelReadyPollingRate, true, 0.0f);
}

void UCutsceneSubsystem::PreLoadCutscene(TSoftObjectPtr<UWorld> CutsceneSublevel, TSoftObjectPtr<ULevelSequence> CutsceneLevelSequence)
{
	Sequence = CutsceneLevelSequence.LoadSynchronous();
	
	const FString LevelName = CutsceneSublevel.GetAssetName();
	CutsceneSublevelName = *LevelName;
	
	UGameplayStatics::LoadStreamLevel(
		this,
		CutsceneSublevelName,
		false,
		false,
		FLatentActionInfo()
	);
}

void UCutsceneSubsystem::RemoveTransitionWidget()
{
	if (!TransitionWidget)
	{
		return;
	}

	TransitionWidget->RemoveFromParent();
	TransitionWidget = nullptr;
}

void UCutsceneSubsystem::TryPlayCutscene()
{
	ElapsedWaitTime += IsSublevelReadyPollingRate;
	if (ElapsedWaitTime >= MaxWaitTime)
	{
		TimerManager->ClearTimer(TimerHandle);
	}
	
	ULevelStreaming* LoadedLevel = UGameplayStatics::GetStreamingLevel(World, CutsceneSublevelName);
	if (LoadedLevel && LoadedLevel->IsLevelLoaded() && LoadedLevel->IsLevelVisible())
	{
		//clear timer
		TimerManager->ClearTimer(TimerHandle);
		
		// Start cutscene
		if (Sequence)
		{
			FMovieSceneSequencePlaybackSettings Settings;
			Settings.bHidePlayer = true;
			ALevelSequenceActor* OutActor = nullptr;
			Player = ULevelSequencePlayer::CreateLevelSequencePlayer(World, Sequence, Settings, OutActor);
			if (Player)
			{				
				Player->OnFinished.AddDynamic(this, &ThisClass::OnCutsceneFinished);
				Player->OnStop.AddDynamic(this, &ThisClass::OnCutsceneFinished);

				OnCutsceneStart.Broadcast();

				// mute SFXs
				UCustomGameInstance* GI = Cast<UCustomGameInstance>(GetGameInstance());
				if (GI)
				{
					PreviousSFXVolume = GI->SFXSoundClass->Properties.Volume;
					UGameplayStatics::SetSoundMixClassOverride(this, GI->SoundMix, GI->SFXSoundClass, 0.0f);
				}

				Player->Play();
				SetUpCutsceneSkip();
			}
		}
	}
}

void UCutsceneSubsystem::OnCutsceneFinished()
{	
	Player->OnFinished.RemoveDynamic(this, &ThisClass::OnCutsceneFinished);
	Player->OnStop.RemoveDynamic(this, &ThisClass::OnCutsceneFinished);
	
	ClearSkipCutscene();
	
	if (!World || CurrentLatentUUID == -1)
	{
		return;
	}
	
	// unload sublevel
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnSublevelUnloaded");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;	// Garantisce un ID univoco all’azione
	
	UGameplayStatics::UnloadStreamLevel(World, CutsceneSublevelName, LatentInfo, false);
}

void UCutsceneSubsystem::OnSublevelUnloaded()
{	
	CutsceneSublevelName = NAME_None;
	Sequence = nullptr;

	if (!World || CurrentLatentUUID == -1)
	{
		return;
	}

	FLatentAction_PlayCutscene* Action = World->GetLatentActionManager().FindExistingAction<FLatentAction_PlayCutscene>(CutsceneLatentCallbackTarget, CurrentLatentUUID);
	if (Action)
	{
		// unmute SFXs
		UCustomGameInstance* GI = Cast<UCustomGameInstance>(GetGameInstance());
		if (GI)
		{
			UGameplayStatics::SetSoundMixClassOverride(this, GI->SoundMix, GI->SFXSoundClass, PreviousSFXVolume);
		}
		
		Action->bIsFinished = true;
		World->GetLatentActionManager().ProcessLatentActions(CutsceneLatentCallbackTarget, 0.0f);
		CurrentLatentUUID = -1;
	}
}

void UCutsceneSubsystem::HandleSkipCutsceneRequested()
{
	OnSkipRequested.Broadcast();
	Player->Stop();
}

void UCutsceneSubsystem::SetUpCutsceneSkip()
{
	UCustomGameInstance* GI = Cast<UCustomGameInstance>(GetGameInstance());
	if (GI)
	{
		SkipWidget = CreateWidget<USkipCutsceneWidget>(GetWorld(), GI->SkipCutsceneWidgetClass);
	}
	
	if (SkipWidget)
	{
		SkipWidget->AddToViewport(9999);
		SkipWidget->OnSkipCutsceneEvent.AddDynamic(this, &ThisClass::HandleSkipCutsceneRequested);
	}
}

void UCutsceneSubsystem::ClearSkipCutscene()
{
	if (!SkipWidget)
	{
		return;
	}
	
	SkipWidget->OnSkipCutsceneEvent.RemoveDynamic(this, &ThisClass::HandleSkipCutsceneRequested);
	SkipWidget->RemoveFromParent();
	SkipWidget = nullptr;
}
