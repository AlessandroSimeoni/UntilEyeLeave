#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonRichTextBlock.h"
#include "CheckpointListEntry.generated.h"

class ACheckpoint;
class UMapWidget;
class UCheckpointMapButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckpointEntryFocused, UCheckpointListEntry*, This);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckpointEntryClicked, UCheckpointListEntry*, This);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckpointEntryUnFocused, UCheckpointListEntry*, This);

UCLASS()
class TBO_01_API UCheckpointListEntry : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	ACheckpoint* BindedCheckpoint = nullptr;
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CheckpointName;

	UPROPERTY(BlueprintAssignable, Category="Checkpoint")
	FOnCheckpointEntryFocused OnCheckpointEntryFocused;
	UPROPERTY(BlueprintAssignable, Category="Checkpoint")
	FOnCheckpointEntryClicked OnCheckpointEntryClicked;
	UPROPERTY(BlueprintAssignable, Category="Checkpoint")
	FOnCheckpointEntryUnFocused OnCheckpointEntryUnFocused;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Checkpoint")
	UMapWidget* MapWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Checkpoint")
	int32 IndexValue;

	UFUNCTION(BlueprintCallable)
	void HandleClicked();
	UFUNCTION(BlueprintCallable)
	void HandleFocused();
	UFUNCTION(BlueprintCallable)
	void HandleUnFocused();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	//virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
