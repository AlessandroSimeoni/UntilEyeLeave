#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IconTableRow.generated.h"

USTRUCT(BlueprintType)
struct FIconTableRow: public FTableRowBase
{
	GENERATED_BODY()

public:

	FIconTableRow()
		:
		EntryImage(nullptr),
		CollectedImage(nullptr),
		IconName("")
	{}

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	UTexture2D* EntryImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	UTexture2D* CollectedImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	FName IconName;

};
