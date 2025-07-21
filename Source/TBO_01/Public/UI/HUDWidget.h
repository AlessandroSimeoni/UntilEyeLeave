// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UInteractionWithPriceWidget;
class UCommonActionWidget;
class AMainCharacter;
/**
 * 
 */
UCLASS()
class TBO_01_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	AMainCharacter* MainCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Interaction", Meta = (BindWidget))
	TObjectPtr<UInteractionWithPriceWidget> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Health", Meta = (BindWidget))
	class UPlayerHealthBar* HealthBar;
	
protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> InteractionPromptComponent = nullptr;
	UPROPERTY()
	TObjectPtr<APlayerController> PC = nullptr;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void HandleInteractionInRange(bool Value, USceneComponent* InteractionPromptLocationComponent);
};
