// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestiaryTableRow.h"
#include "Journal/NewBaseWidgetClass.h"
#include "BestiarySection.generated.h"

class UBestiaryPageWidget;
enum class EPageType : uint8;
class UEntryJournalButton;
class UActionWidget;

UCLASS()
class TBO_01_API UBestiarySection : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UVerticalBox* CreatureList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "Actions")
	UActionWidget* UpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "Actions")
	UActionWidget* DownAction;
	UFUNCTION(BlueprintCallable)
	void AddToBestiaryButtons(UEntryJournalButton* Button);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UEntryJournalButton*> BestiaryButtons = TArray<UEntryJournalButton*>();
	UPROPERTY()
	int32 FocusIndex = 0;
protected:
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	UFUNCTION(BlueprintCallable)
	void SetFocusIndex(int32 Index);
};
