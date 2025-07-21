#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "Credits.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UCredits : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true, UIMin = 0.0f, ClampMin = 0.0f))
	float CloseCooldownTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FKey KeyboardCloseKey;
	
	float OpenedTime = 0.0f;
	
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
