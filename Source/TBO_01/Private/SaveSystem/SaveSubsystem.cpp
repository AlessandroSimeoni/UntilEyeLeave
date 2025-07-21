// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/SaveSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/TBOSaveGame.h"
#include "SaveSystem/TBOSaveSettings.h"

void USaveSubsystem::SaveGame()
{
	if (!TBOSaveGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Saving: TBOSaveGame does not exist! Creating one..."));
		TBOSaveGame = Cast<UTBOSaveGame>(UGameplayStatics::CreateSaveGameObject(UTBOSaveGame::StaticClass()));
	}

	UGameplayStatics::SaveGameToSlot(TBOSaveGame, GameSlotName, UserIndex);
	OnSaveComplete.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("Saving: Game Save Completed"));
}

UTBOSaveGame* USaveSubsystem::LoadGame()
{
	if (TBOSaveGame)
		return TBOSaveGame;

	if (DoesSaveExist(GameSlotName))
		TBOSaveGame =  Cast<UTBOSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSlotName, UserIndex));
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading: TBOSaveGame does not exist! Creating one..."));
		TBOSaveGame = Cast<UTBOSaveGame>(UGameplayStatics::CreateSaveGameObject(UTBOSaveGame::StaticClass()));
	}

	OnLoadComplete.Broadcast();
	
	UE_LOG(LogTemp, Warning, TEXT("Loading: Load Completed"));

	return TBOSaveGame;
}

void USaveSubsystem::SaveSettings()
{
	if (!TBOSaveSettings)
	{
		UE_LOG(LogTemp, Warning, TEXT("Saving: TBOSaveSettings does not exist! Creating one..."));
		TBOSaveSettings = Cast<UTBOSaveSettings>(UGameplayStatics::CreateSaveGameObject(UTBOSaveSettings::StaticClass()));
	}

	UGameplayStatics::SaveGameToSlot(TBOSaveSettings, SettingsSlotName, UserIndex);

	UE_LOG(LogTemp, Warning, TEXT("Saving: Settings Save Completed"));
}

UTBOSaveSettings* USaveSubsystem::LoadSettings()
{
	if (TBOSaveSettings)
		return TBOSaveSettings;

	if (DoesSaveExist(SettingsSlotName))
		TBOSaveSettings =  Cast<UTBOSaveSettings>(UGameplayStatics::LoadGameFromSlot(SettingsSlotName, UserIndex));
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading: TBOSaveSettings does not exist! Creating one..."));
		TBOSaveSettings = Cast<UTBOSaveSettings>(UGameplayStatics::CreateSaveGameObject(UTBOSaveSettings::StaticClass()));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Loading: Settings Load Completed"));

	return TBOSaveSettings;
}

void USaveSubsystem::DeleteSave()
{
	if (DoesSaveExist(GameSlotName))
	{
		UGameplayStatics::DeleteGameInSlot(GameSlotName, UserIndex);
		TBOSaveGame = nullptr;
		OnDeleteComplete.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Deleting: GameSave deleted"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Deleting: no GameSave to delete!"));
	}
}

bool USaveSubsystem::DoesSaveExist(FString SlotName) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

void USaveSubsystem::CleanSaveGamePointer()
{
	TBOSaveGame = nullptr;
}
