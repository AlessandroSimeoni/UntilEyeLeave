#pragma once

#include "CoreMinimal.h"
#include "BestiaryPageWidget.h"
#include "BestiaryTableRow.h"
#include "CommonButtonBase.h"
#include "CommonRichTextBlock.h"
#include "EntryJournalButton.generated.h"

class UBestiarySection;

UCLASS()
class TBO_01_API UEntryJournalButton : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UCommonRichTextBlock* EntryTitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* EntryIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* FragmentDiscovered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* QuestionMarks;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDataTableRowHandle EntryRow;
	UFUNCTION(BlueprintCallable)
	void UpdateJournalPage(UBestiaryPageWidget* Page);
	UFUNCTION(BlueprintCallable)
	UEntryJournalButton* UpdateButtonInfo();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBestiarySection* Section;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IndexValue;
	UFUNCTION(BlueprintCallable)
	void PlayFadeSound(UBestiaryPageWidget* Page);
	UFUNCTION(BlueprintCallable)
	void StopFadeSound(UBestiaryPageWidget* Page);
	UFUNCTION(BlueprintCallable)
	void InitializeButton(FBestiaryTableRow InRow);
	
private:
	FBestiaryTableRow Row;
	bool PlaySound = false;
protected:
	UFUNCTION(BlueprintCallable)
	void UpdateDiscoveryIcon();
};


