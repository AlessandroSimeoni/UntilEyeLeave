#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BestiaryTableRow.generated.h"

UENUM(BlueprintType)
enum class EPageType : uint8
{
	Map       UMETA(DisplayName = "Map"),
	Bestiary  UMETA(DisplayName = "Bestiary"),
	Lore      UMETA(DisplayName = "Lore")
};

UENUM(BlueprintType)
enum class ECreatureType : uint8
{
	Passive       UMETA(DisplayName = "Passive"),
	Aggressive  UMETA(DisplayName = "Aggressive"),
	Both      UMETA(DisplayName = "Both")
};

USTRUCT(BlueprintType)
struct FBestiaryTableRow: public FTableRowBase
{
	GENERATED_BODY()

public:

	FBestiaryTableRow()
		: Section(EPageType::Map)
		, EntryTitle(FText::GetEmpty())
		, CreatureType(ECreatureType::Passive)
		, EntryIcon(nullptr)
		, EntryImage(nullptr)
		, TemperIcon(nullptr)
		, ProtagonistNoteLeftSide(FText::GetEmpty())
		, ProtagonistNoteRightSide(FText::GetEmpty())
		, Hint1(FText::GetEmpty())
		, Hint2(FText::GetEmpty())
		, Hint3(FText::GetEmpty())
		, Hint4(FText::GetEmpty())
		, Lore(FText::GetEmpty())
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	EPageType Section;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FText EntryTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	ECreatureType CreatureType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	UTexture2D* EntryIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	UTexture2D* EntryImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	UTexture2D* TemperIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FText ProtagonistNoteLeftSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FText ProtagonistNoteRightSide;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FText Hint1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FText Hint2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FText Hint3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FText Hint4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FText Lore;
};

