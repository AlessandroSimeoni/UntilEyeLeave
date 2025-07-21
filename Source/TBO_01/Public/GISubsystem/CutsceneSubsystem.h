// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CutsceneSubsystem.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class USkipCutsceneWidget;
class ULevelSequencePlayer;
class ULevelSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCutsceneStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkipRequested);

UCLASS()
class TBO_01_API UCutsceneSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FTimerManager* TimerManager;
	FTimerHandle TimerHandle;
	FName CutsceneSublevelName = NAME_None;
	float IsSublevelReadyPollingRate = 0.1f;
	float MaxWaitTime = 10.0f;
	float ElapsedWaitTime = 0.0f;
	int32 CurrentLatentUUID = -1;
	float PreviousSFXVolume = 0.0f;
	UPROPERTY()
	TObjectPtr<USkipCutsceneWidget> SkipWidget = nullptr;
	UPROPERTY()
	UWorld* World = nullptr;
	UPROPERTY()
	ULevelSequence* Sequence = nullptr;
	UPROPERTY()
	ULevelSequencePlayer* Player;
	UPROPERTY()
	UObject* CutsceneLatentCallbackTarget = nullptr;

	UFUNCTION()
	void TryPlayCutscene();
	UFUNCTION()
	void OnCutsceneFinished();
	UFUNCTION()
	void OnSublevelUnloaded();
	UFUNCTION()
	void HandleSkipCutsceneRequested();
	void SetUpCutsceneSkip();
	void ClearSkipCutscene();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Cutscene", Meta = (WorldContext = "WorldContext", Latent, LatentInfo = "LatentInfo"))
	void PlayCutscene(UObject* WorldContext, FLatentActionInfo LatentInfo, TSoftObjectPtr<UWorld> CutsceneSublevel, TSoftObjectPtr<ULevelSequence> CutsceneLevelSequence);
	UFUNCTION(BlueprintCallable, Category = "Cutscene")
	void PreLoadCutscene(TSoftObjectPtr<UWorld> CutsceneSublevel, TSoftObjectPtr<ULevelSequence> CutsceneLevelSequence);
	UFUNCTION(BlueprintCallable, Category = "Cutscene")
	void RemoveTransitionWidget();
	
	
	UPROPERTY(BlueprintAssignable, Category = "Cutscene")
	FOnCutsceneStart OnCutsceneStart;
	UPROPERTY(BlueprintAssignable, Category = "Cutscene")
	FOnSkipRequested OnSkipRequested;

	UPROPERTY(BlueprintReadWrite, Category = "Cutscene")
	bool ComingFromMainMenu = false;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> TransitionWidget = nullptr;
};
