#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "OverlayBlockZoom.generated.h"

class UMapWidget;

UCLASS()
class TBO_01_API UOverlayBlockZoom : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	UMapWidget* MapWidget;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
