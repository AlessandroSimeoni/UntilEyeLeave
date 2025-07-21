// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingTransition.generated.h"

/**
 * 
 */

class USceneHandlerGISubsystem;

UCLASS()
class TBO_01_API ULoadingTransition : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Loading Functions")
	void StartLoading(float MinDuration);
	UFUNCTION(BlueprintCallable, Category = "Loading Functions")
	void EndLoading();

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* StartLoadingAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* EndLoadingAnimation;
	UPROPERTY()
	USceneHandlerGISubsystem* SceneHandlerGISubsystem = nullptr;

	bool LoadingCooldown = false;
	bool TargetMapReady = false;
	FTimerHandle LoadingTimerHandle;
	FTimerManager* TM;

	UFUNCTION(BlueprintCallable, Category = "Loading Handling Functions")
	void HandleStartLoadingOver();
	UFUNCTION(BlueprintCallable, Category = "Loading Handling Functions")
	void HandleEndLoadingOver();
	UFUNCTION()
	void MapLoadedCallback(FSoftObjectPath LoadedMapPath);
	UFUNCTION()
	void RequestMapOpening();
};
