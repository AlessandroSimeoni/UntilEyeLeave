#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ActionWidget.generated.h"

class UCommonActionWidget;
/**
 * 
 */
UCLASS()
class TBO_01_API UActionWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonActionWidget* ActionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	FKey KeyboardKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	FKey GamepadKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Animation")
	UWidgetAnimation* Animation;
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayActionAnimation();
};
