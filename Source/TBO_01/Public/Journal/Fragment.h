#pragma once

#include "CoreMinimal.h"
#include "CollectableItems.h"
#include "GameFramework/Actor.h"
#include "Journal/BestiaryTableRow.h"
#include "Fragment.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnFragmentCollected, FName, EntryId, EPageType, Section, TArray<int32>, Indexes, bool, IsTrigger);

class AJournalManager;

UCLASS()
class TBO_01_API AFragment : public ACollectableItems
{
	GENERATED_BODY()
	
public:	
	AFragment();


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fragment")
	FName RepresentedEntryID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fragment")
	EPageType Section;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fragment")
	TArray<int32> FragmentsIndexes;

	//UPROPERTY(BlueprintAssignable)
	//FOnFragmentCollected OnFragmentCollected;
	
	UFUNCTION(BlueprintCallable, Category="Fragment")
	void PickUpFragment();
	
	UFUNCTION(BlueprintCallable)
	virtual void Collected() override;

	UFUNCTION(BlueprintCallable)
	virtual void Discovered() override;
private:
	UPROPERTY()
	AJournalManager* JournalManager = nullptr;
};
