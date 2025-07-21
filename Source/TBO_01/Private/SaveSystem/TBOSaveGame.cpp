// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/TBOSaveGame.h"

#include "Journal/JournalEntry.h"

void UTBOSaveGame::UnlockTeleportPoint(FGuid CheckpointGuid, FVector TeleportLocation)
{
	UnlockedTeleportPoints.Add(CheckpointGuid, TeleportLocation);
	AddInteractedActor(CheckpointGuid);
}

void UTBOSaveGame::AddInteractedActor(const FGuid ActorGuid)
{
	InteractedActors.AddUnique(ActorGuid);
}

void UTBOSaveGame::RemoveInteractedActor(const FGuid ActorGuid)
{
	InteractedActors.RemoveSingle(ActorGuid);
}

bool UTBOSaveGame::FindInteractedActor(const FGuid ActorGuid)
{
	return InteractedActors.Contains(ActorGuid);
}

void UTBOSaveGame::AddDiscoverCollectible(ACollectableItems* Collectable)
{
	DiscoveredCollectibles.Add(Collectable);
}

void UTBOSaveGame::RemovedDiscoveredCollectible(ACollectableItems* Collectable)
{
	DiscoveredCollectibles.Remove(Collectable);
}

void UTBOSaveGame::AddJournalEntry(FName Name)
{
	FJournalEntry NewEntry;
	NewEntry.EntryID = Name;
	NewEntry.IsDiscovered = true;
	PersistentJournalEntries.Add(Name, NewEntry);
}

void UTBOSaveGame::AddMapFragment(TArray<int32> Fragments)
{
	MapFragments.Append(Fragments);
}

void UTBOSaveGame::AppendJournalEntryIndexes(FName EntryID,TArray<int32> Fragments)
{
	FJournalEntry& Entry = PersistentJournalEntries[EntryID];
	Entry.FragmentsCollected.Append(Fragments);
}

void UTBOSaveGame::UpdateEntriesFragmentsUnlocked(FName Name, int32 Index)
{
	FIntArray& Fragments = EntriesFragmentUnlocked[Name];
	Fragments . IntArrayCollected.Add(Index);
}

void UTBOSaveGame::CreateEntriesFragmentsUnlocked(FName Name)
{
	FIntArray Fragments;
	EntriesFragmentUnlocked.Add(Name, Fragments);
}

void UTBOSaveGame::AddMapFragmentsUnlocked(int32 Index)
{
	MapFragmentsUnlocked.Add(Index);
}

//currency
void UTBOSaveGame::AddCoin(const FGuid ActorGuid)
{
	Coins.AddUnique(ActorGuid);
}

bool UTBOSaveGame::FindCoinActor(const FGuid ActorGuid)
{
	return Coins.Contains(ActorGuid);
}

