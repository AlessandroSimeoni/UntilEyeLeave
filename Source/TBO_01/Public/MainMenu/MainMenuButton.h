#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MainMenuButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonSelection, ACameraActor*, Camera);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, UMainMenuButton*, Button);

UCLASS()
class TBO_01_API UMainMenuButton : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCommonRichTextBlock* ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACameraActor* BindCamera;

	UPROPERTY(BlueprintAssignable)
	FOnButtonSelection OnButtonSelection;
	UPROPERTY(BlueprintAssignable)
	FOnButtonClicked OnButtonClicked;
	
	UFUNCTION(BlueprintCallable)
	void SetBindCameraActor(class ACameraActor* CameraActor);

	UFUNCTION(BlueprintCallable)
	void HandleSelection();
	UFUNCTION(BlueprintCallable)
	void HandleClicked();
	
};
