// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioSubsystem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESFXType : uint8
{
	SFX_MainMenu     = 0     UMETA(DisplayName = "Main Menu"),
	SFX_Gameplay     = 1     UMETA(DisplayName = "Gameplay"),
	SFX_Combat       = 2     UMETA(DisplayName = "Combat"),
	SFX_Boss         = 3     UMETA(DisplayName = "Boss"),
	SFX_Cutscene1    = 4     UMETA(DisplayName = "Cutscene 1"),
	SFX_Cutscene2    = 5     UMETA(DisplayName = "Cutscene 2"),
	SFX_Cutscene3    = 6     UMETA(DisplayName = "Cutscene 3"),
	SFX_Extra1       = 7     UMETA(DisplayName = "Extra 1"),
	SFX_Extra2       = 8     UMETA(DisplayName = "Extra 2")
};

UCLASS(Blueprintable,Abstract)
class TBO_01_API UAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	UFUNCTION(BlueprintCallable)
	void SimulateLevelLoadForEditor();
	void OnLevelChangedFromParams(const FActorsInitializedParams& Params);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void ChangeMusic(ESFXType Music);
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void ActivateMetaSound();
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetCutOff(float CutOffValue);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void AddSpottedCombatAudio();
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void RemoveSpottedCombatAudio();
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void RemoveCompletelySpottedCombatAudio();
	bool bHasInitializedAudio = false;

	
protected:
	void OnLevelChanged(UWorld* LoadedWorld);
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TSoftObjectPtr<USoundBase> MetaSoundSourceAsset;
	
	TWeakObjectPtr<UAudioComponent> MetaSoundManager = nullptr;
	
	UPROPERTY(EditDefaultsOnly,category="Audio")
	FName NewTrackParameter = "NewTrack";
	UPROPERTY(EditDefaultsOnly,category="Audio")
	FName TriggerMusicPlayParameter = "TriggerMusicPlay";
	UPROPERTY(EditDefaultsOnly,category="Audio")
	FName CutoffFrequencyParameter = "Cutoff Frequency";

	int32 SpottedEnemies = 0;
};

