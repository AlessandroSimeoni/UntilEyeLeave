// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "Options.generated.h"

class UOptionsTab;
/**
 * 
 */
UCLASS()
class TBO_01_API UOptions : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOptionsTab* W_OptionsTabList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true, UIMin = 0.0f, ClampMin = 0.0f))
	float CloseCooldownTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FKey KeyboardCloseKey;

	float OpenedTime = 0.0f;
protected:
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
