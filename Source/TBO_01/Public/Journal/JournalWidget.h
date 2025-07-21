// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "JournalWidget.generated.h"

class UJournalTab;
class UActionWidget;
/**
 * 
 */
UCLASS()
class TBO_01_API UJournalWidget : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UJournalTab* JTab;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FKey CloseKeyKeyboard = EKeys::Tab;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FKey CloseKeyPad = EKeys::Gamepad_Special_Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	float CloseCooldownTime = 1.0f;
private:
	float OpenedTime;
public:
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
