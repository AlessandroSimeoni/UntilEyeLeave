// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

class UTBOSaveSettings;
class UTBOSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeleteComplete);

UCLASS()
class TBO_01_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UTBOSaveGame> TBOSaveGame = nullptr;
	UPROPERTY()
	TObjectPtr<UTBOSaveSettings> TBOSaveSettings = nullptr;
	
	FString GameSlotName = TEXT("PlayerSaveSlot");
	FString SettingsSlotName = TEXT("GameSettingsSlot");
	int UserIndex = 0;

public:
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	UTBOSaveGame* LoadGame();
	UFUNCTION(BlueprintCallable)
	void SaveSettings();
	UFUNCTION(BlueprintCallable)
	UTBOSaveSettings* LoadSettings();
	UFUNCTION(BlueprintCallable)
	void DeleteSave();
	UFUNCTION(BlueprintCallable)
	bool DoesSaveExist(FString SlotName = TEXT("PlayerSaveSlot")) const;
	UFUNCTION()
	void CleanSaveGamePointer();

	UPROPERTY(BlueprintAssignable)
	FLoadComplete OnLoadComplete;
	UPROPERTY(BlueprintAssignable)
	FSaveComplete OnSaveComplete;
	UPROPERTY(BlueprintAssignable)
	FDeleteComplete OnDeleteComplete;
};
