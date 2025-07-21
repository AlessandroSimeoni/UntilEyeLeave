#include "GISubsystem/SceneHandlerGISubsystem.h"
#include "Engine/AssetManager.h"
#include "AssetRegistry/ARFilter.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/World.h"
#include "UI/LoadingTransition.h"

void USceneHandlerGISubsystem::SceneLoadingTransition(TSoftObjectPtr<UWorld> TargetMap, TSubclassOf<ULoadingTransition> LoadingWidgetClass, float LoadingMinDuration)
{
    CurrentLoadingWidgetClass = LoadingWidgetClass;
    SceneLoadingRequested = true;
    CachedMapToLoad = TargetMap;
    
    ULoadingTransition* LoadingTransitionWidget = CreateWidget<ULoadingTransition>(GetWorld(), LoadingWidgetClass);
    if (LoadingTransitionWidget)
    {
        LoadingTransitionWidget->AddToViewport(1000);
        LoadingTransitionWidget->StartLoading(LoadingMinDuration);
    }
}

void USceneHandlerGISubsystem::LoadCachedMapAsync()
{
    LoadMapAsync(CachedMapToLoad);
}

void USceneHandlerGISubsystem::LoadMapAsync(TSoftObjectPtr<UWorld> MapToLoad)
{
    CachedMapToLoad = MapToLoad;
    if (!CachedMapToLoad.IsPending() && !CachedMapToLoad.IsValid())
    {
        return;
    }
    FSoftObjectPath MapPath = CachedMapToLoad.ToSoftObjectPath();
    FStreamableManager& SM = UAssetManager::GetStreamableManager();

    MapLoadingHandle = SM.RequestAsyncLoad(
        MapPath,
        FStreamableDelegate::CreateLambda(
            [this, MapPath]()
            {
                MapLoaded = true;
                LoadedMapPath = MapPath;

                OnMapLoadComplete.Broadcast(LoadedMapPath);
            }
        ),
        FStreamableManager::DefaultAsyncLoadPriority
    );
    
    GetWorld()->GetTimerManager().SetTimer(ProgressCheckHandle, this, &USceneHandlerGISubsystem::CheckLoadProgress, 0.1f, true);
}

void USceneHandlerGISubsystem::OpenLoadedMap()
{
    if (MapLoaded && LoadedMapPath.IsValid())
    {
        LoadedMapToUnload = GetWorld();
        FString AssetPath = LoadedMapPath.GetAssetPathString();

        if (SceneLoadingRequested)
            FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::HandleLoadingOnMapReady);
        
        UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), CachedMapToLoad);
        
        if (LoadedMapPath == StoredMap.ToSoftObjectPath())
        {
            StoredMap.Reset();
        }
        MapLoadingHandle.Reset();
    }
    MapLoaded = false;
    LoadedMapPath.Reset();
}

void USceneHandlerGISubsystem::HandleLoadingOnMapReady(UWorld* LoadedWorld)
{
    SceneLoadingRequested = false;
    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

    ULoadingTransition* LoadingWidget = CreateWidget<ULoadingTransition>(LoadedWorld, CurrentLoadingWidgetClass);
    if (LoadingWidget)
    {
        LoadingWidget->AddToViewport(1000);
        LoadingWidget->EndLoading();
    }
}

void USceneHandlerGISubsystem::CheckLoadProgress()
{
    if (MapLoadingHandle.IsValid())
    {
        // Check if the load has completed
        if (MapLoadingHandle->HasLoadCompleted())
        {
            OnMapLoadProgress.Broadcast(1.0f);  // Broadcast completion at 100% progress
            GetWorld()->GetTimerManager().ClearTimer(ProgressCheckHandle);
            MapLoadingHandle.Reset();
        }
        else
        {
            // Get the progress of the current load operation
            float Progress = MapLoadingHandle->GetProgress();
            OnMapLoadProgress.Broadcast(Progress);  // Broadcast progress update
        }
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(ProgressCheckHandle);
    }
}

/*
void USceneHandlerGISubsystem::UnloadMap(TSoftObjectPtr<UWorld> MapToUnload)
{
    CachedMapToUnload = MapToUnload;
    
    if (!CachedMapToUnload.IsValid())
    {
        return;
    }
    
    FStreamableManager& SM = UAssetManager::GetStreamableManager();
    const FSoftObjectPath MapPath = CachedMapToUnload.ToSoftObjectPath();
    SM.Unload(MapPath);
    
    MapLoaded = false;
    OnMapUnloadComplete.Broadcast(MapPath);
    MapLoadingHandle.Reset();
    LoadedMapToUnload.Reset();
}
 */

TSoftObjectPtr<UWorld> USceneHandlerGISubsystem::GetCurrentWorld() const
{
    FString CurrentWorldPath = GetWorld()->GetOuter()->GetPathName();
    
    int Index;
#if WITH_EDITOR
    CurrentWorldPath = *UWorld::RemovePIEPrefix(CurrentWorldPath);
#endif
    CurrentWorldPath.FindLastChar('/', Index);
    FString LevelName = CurrentWorldPath.Mid(Index+1);
    CurrentWorldPath = CurrentWorldPath.Append("." + LevelName);
    
    FSoftObjectPath WorldPath(*CurrentWorldPath);
    TSoftObjectPtr<UWorld> SoftWorld(WorldPath);

    return SoftWorld;
}
