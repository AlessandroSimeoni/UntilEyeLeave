// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Journal/JournalEntry.h"
#include "TBOSaveGame.generated.h"

class ACollectableItems;

UCLASS()
class TBO_01_API UTBOSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	FVector LastCheckpointLocation = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	TMap<FGuid, FVector> UnlockedTeleportPoints = TMap<FGuid, FVector>();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interacted Actors")
	TArray<FGuid> InteractedActors = TArray<FGuid>();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interacted Actors")
	TArray<ACollectableItems*> DiscoveredCollectibles = TArray<ACollectableItems*>();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fragments")
	TMap<FName, FJournalEntry> PersistentJournalEntries = TMap<FName, FJournalEntry>();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
	TArray<int32> MapFragments = TArray<int32>();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JournalEntries")
	TMap<FName, FIntArray> EntriesFragmentUnlocked = TMap<FName, FIntArray>();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
	TArray<int32> MapFragmentsUnlocked = TArray<int32>();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Life Shards")
	int32 LifeShardsCollected = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
	int32 WeaponPartsCollected = 0;

	//Currency
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Currency")
	int32 CoinsCollected = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interacted Actors")
	TArray<FGuid> Coins = TArray<FGuid>();
	
	UFUNCTION(BlueprintCallable)
	void UnlockTeleportPoint(FGuid CheckpointGuid, FVector TeleportLocation);
	UFUNCTION(BlueprintCallable)
	void AddInteractedActor(const FGuid ActorGuid);
	UFUNCTION(BlueprintCallable)
	void RemoveInteractedActor(const FGuid ActorGuid);
	UFUNCTION(BlueprintCallable)
	bool FindInteractedActor(const FGuid ActorGuid);
	UFUNCTION(BlueprintCallable)
	void AddDiscoverCollectible(ACollectableItems* Collectable);
	UFUNCTION(BlueprintCallable)
	void RemovedDiscoveredCollectible(ACollectableItems* Collectable);
	UFUNCTION(BlueprintCallable)
	void AddJournalEntry(FName Name);
	UFUNCTION(BlueprintCallable)
	void AddMapFragment(TArray<int32> Fragments);
	UFUNCTION(BlueprintCallable)
	void AppendJournalEntryIndexes(FName EntryID, TArray<int32> Fragments);
	UFUNCTION()
	void UpdateEntriesFragmentsUnlocked(FName Name, int32 Index);
	UFUNCTION()
	void CreateEntriesFragmentsUnlocked(FName Name);
	UFUNCTION()
	void AddMapFragmentsUnlocked(int32 Index);

	//currency
	UFUNCTION(BlueprintCallable)
	void AddCoin(const FGuid ActorGuid);
	UFUNCTION(BlueprintCallable)
	bool FindCoinActor(const FGuid ActorGuid);
};

