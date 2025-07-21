#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "MainMenuOpeningWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnyKeyPressed);

UCLASS()
class TBO_01_API UMainMenuOpeningWidget : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UCommonRichTextBlock* TextBlock;

	UPROPERTY(BlueprintAssignable)
	FAnyKeyPressed AnyKeyPressedEvent;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
