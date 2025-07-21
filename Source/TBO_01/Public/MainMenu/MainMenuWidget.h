#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "MainMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLerpRequest, ACameraActor*, Camera);

UCLASS()
class TBO_01_API UMainMenuWidget : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UMainMenuButton* NewGameButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UMainMenuButton* ContinueButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UMainMenuButton* OptionsButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UMainMenuButton* CreditsButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UMainMenuButton* QuitButton;
	
	UPROPERTY(BlueprintAssignable)
	FOnLerpRequest OnLerpRequest;
	
	virtual void NativeOnActivated() override;

	UFUNCTION()
	void StartLerp(ACameraActor* Camera);
protected:
	UPROPERTY(BlueprintReadWrite)
	class UMainMenuButton* LastClickedButton = nullptr;

private:
	UFUNCTION()
	void SetLastClick(UMainMenuButton* Button);
	UFUNCTION(BlueprintCallable)
	void ResetLastClickedButton();
	UFUNCTION(BlueprintCallable)
	void InitialButtonFocus();
};

