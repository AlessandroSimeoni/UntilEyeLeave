// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHeart.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeartFilled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeartEmptied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHeartCreatedOrDestroyed, UPlayerHeart*, ThisHeart, bool, IsCreated);

UCLASS()
class TBO_01_API UPlayerHeart : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Heart", Meta = (BindWidget))
	class UImage * HeartImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Heart")
	class UTexture2D * EmptyHeartSprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Heart")
	class UTexture2D * HalfHeartSprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Heart")
	class UTexture2D * FullHeartSprite;

	UFUNCTION(BlueprintCallable, Category = "Heart Slot Functions")
	void FillHeartByValue(float Value);
	UFUNCTION(BlueprintCallable, Category = "Heart Slot Functions")
	void PlayFillHeartAnim(float Value);
	UFUNCTION(BlueprintCallable, Category = "Heart Slot Functions")
	void EmptyHeartByValue(float Value);
	UFUNCTION(BlueprintCallable, Category = "Heart Slot Functions")
	void PlayEmptyHeartAnim(float Value);
	UFUNCTION(BlueprintCallable, Category = "Heart Slot Functions")
	void PlayHeartCreationAnim(float Value);
	UFUNCTION(BlueprintCallable, Category = "Heart Slot Functions")
	void PlayHeartDestructionAnim();
	UFUNCTION(BlueprintCallable, Category = "Heart Slot Functions")
	void BroadcastCreationOrDestruction(const bool IsCreation);
	
	UPROPERTY(BlueprintAssignable)
	FHeartFilled OnHeartFilled;
	UPROPERTY(BlueprintAssignable)
	FHeartEmptied OnHeartEmptied;
	UPROPERTY(BlueprintAssignable)
	FHeartCreatedOrDestroyed OnHeartCreatedOrDestroyed;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* HealAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* DamageAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* CreationAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* DestructionAnimation;
	
	UFUNCTION(BlueprintCallable, Category = "Heart Slot Functions")
	void UpdateHeart(bool IsHeal) const;

	float RequestedValue = -1.0f;
};
