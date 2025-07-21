#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "ConfirmationModalWidget.generated.h"

class UCheckpointMapButton;
class UCommonButtonBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmation);

UCLASS()
class TBO_01_API UConfirmationModalWidget : public UNewBaseWidgetClass
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable, Category="Confirmation")
	FOnConfirmation OnConfirmed;

	UPROPERTY(BlueprintAssignable, Category="Confirmation")
	FOnConfirmation OnCanceled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCheckpointMapButton* ActivatorButton;
	
protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetOptional))
	UCommonButtonBase* ConfirmButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UCommonButtonBase* CancelButton;

	
	UFUNCTION(BlueprintCallable, Category="Confirmation")
	void HandleConfirmClicked();

	UFUNCTION(BlueprintCallable)
	void HandleCancelClicked();
	virtual void NativeOnActivated() override;
};
