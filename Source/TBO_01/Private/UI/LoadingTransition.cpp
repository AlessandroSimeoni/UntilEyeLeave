// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadingTransition.h"

#include "GISubsystem/CustomGameInstance.h"

void ULoadingTransition::StartLoading(float MinDuration)
{
	if (MinDuration > 0.0f)
	{
		TM = &GetWorld()->GetTimerManager();
		TM->SetTimer(LoadingTimerHandle, this, &ThisClass::RequestMapOpening, MinDuration, false);
		LoadingCooldown = true;
	}
	
	PlayAnimation(StartLoadingAnimation);
}

void ULoadingTransition::EndLoading()
{
	PlayAnimation(EndLoadingAnimation);
}

void ULoadingTransition::HandleStartLoadingOver()
{
	SceneHandlerGISubsystem = Cast<UCustomGameInstance>(GetGameInstance())->GetSceneHandlerSubsystem();
	SceneHandlerGISubsystem->OnMapLoadComplete.AddDynamic(this, &ThisClass::MapLoadedCallback);
	SceneHandlerGISubsystem->LoadCachedMapAsync();
}

void ULoadingTransition::HandleEndLoadingOver()
{
	RemoveFromParent();
}

void ULoadingTransition::MapLoadedCallback(FSoftObjectPath LoadedMapPath)
{
	SceneHandlerGISubsystem->OnMapLoadComplete.RemoveDynamic(this, &ThisClass::MapLoadedCallback);
	TargetMapReady = true;

	if (!LoadingCooldown)
	{
		RequestMapOpening();
		TargetMapReady = false;
	}
}

void ULoadingTransition::RequestMapOpening()
{
	if (TargetMapReady)
	{
		SceneHandlerGISubsystem->OpenLoadedMap();
		TargetMapReady = false;
	}

	LoadingCooldown = false;
	TM->ClearTimer(LoadingTimerHandle);
}
