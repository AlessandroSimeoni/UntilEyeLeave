#pragma once

#include "CoreMinimal.h"
#include "Journal/BestiaryTableRow.h"
#include "JournalEntry.generated.h"

USTRUCT(BlueprintType)
struct FJournalEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Journal")
	FName EntryID;

	UPROPERTY(BlueprintReadOnly, Category = "Journal")
	EPageType FragmentType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Journal")
	bool IsDiscovered;

	UPROPERTY(BlueprintReadOnly, Category = "Journal")
	TArray<int32> FragmentsCollected;

	FJournalEntry()
		: EntryID(NAME_None), FragmentType(EPageType::Bestiary), IsDiscovered(false) {}
};

USTRUCT(BlueprintType)
struct FIntArray
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> IntArrayCollected;

	FIntArray() {}
};