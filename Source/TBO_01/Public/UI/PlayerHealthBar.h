// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthBar.generated.h"

/**
 * 
 */

class ULifeSystemComponent;
class UPlayerHeart;
class AMainCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCompletelyEmptyHealthBar);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmptiedHealthBar);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFilledHealthBar);

UCLASS()
class TBO_01_API UPlayerHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	AMainCharacter* MainCharacter;
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	ULifeSystemComponent* MainCharacterLife;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Heart Container", Meta = (BindWidget))
	class UHorizontalBox* HeartContainer;

	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	void InitHealthBar(AMainCharacter* Character);
	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	void EmptyHealthBar(USceneComponent* InstigatorComponent, float Damage);
	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	void EmptyTotalHealthBar(USceneComponent* InstigatorComponent);
	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	void FillHealthBar(USceneComponent* InstigatorComponent, float HealValue);
	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	void ResizeHealthBar();

	UPROPERTY(BlueprintAssignable)
	FCompletelyEmptyHealthBar OnCompletelyEmptyHealthBar;
	UPROPERTY(BlueprintAssignable)
	FEmptiedHealthBar OnHealthBarEmptied;
	UPROPERTY(BlueprintAssignable)
	FFilledHealthBar OnHealthBarFilled;
	
protected:
	UPROPERTY(EditAnywhere, Category = "HeartWidget")
	TSubclassOf<UPlayerHeart> HeartWidgetClass;

	UFUNCTION()
	void HandleHeartCreatedOrDestroyed(UPlayerHeart* TargetHeart, bool IsCreated);
	
	TArray<UPlayerHeart*> Hearts;
	int CurrentHeartIndex = -1;
	bool WasFullyHealed = true;
};
