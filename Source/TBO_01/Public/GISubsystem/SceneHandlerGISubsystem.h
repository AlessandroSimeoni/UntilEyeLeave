#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "SceneHandlerGISubsystem.generated.h"

class ULoadingTransition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapLoadComplete, FSoftObjectPath, LoadedMapPath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapUnloadComplete, FSoftObjectPath, UnloadedMapPath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapLoadProgress, float, LoadProgress);

UCLASS()
class TBO_01_API USceneHandlerGISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnMapLoadComplete OnMapLoadComplete;
	UPROPERTY(BlueprintAssignable)
	FOnMapUnloadComplete OnMapUnloadComplete;
	UPROPERTY(BlueprintAssignable)
	FOnMapLoadProgress OnMapLoadProgress;
	
	bool MapLoaded = false;
	bool PersistentLevelLoaded = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UWorld> LoadedMapToUnload;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UWorld> StoredMap;
	
	UFUNCTION(BlueprintCallable, Category = "GISubsystem")
	void SceneLoadingTransition(TSoftObjectPtr<UWorld> TargetMap, TSubclassOf<ULoadingTransition> LoadingWidgetClass, float LoadingMinDuration);
	UFUNCTION(BlueprintCallable, Category = "GISubsystem")
	void LoadCachedMapAsync();
	UFUNCTION(BlueprintCallable, Category = "GISubsystem")
	void LoadMapAsync(TSoftObjectPtr<UWorld> MapToLoad);
	UFUNCTION(BlueprintCallable, Category = "GISubsystem")
	void OpenLoadedMap();
	UFUNCTION(BlueprintCallable, Category = "GISubsystem")
	void CheckLoadProgress();
	UFUNCTION(BlueprintCallable, Category = "GISubsystem")
	TSoftObjectPtr<UWorld> GetCurrentWorld() const;
	
/*
	UFUNCTION(BlueprintCallable, Category = "GISubsystem")
	void UnloadMap(TSoftObjectPtr<UWorld> MapToUnload);
*/
	
private:
	FSoftObjectPath LoadedMapPath;
	UPROPERTY()
	TSoftObjectPtr<UWorld> CachedMapToLoad;
	UPROPERTY()
	TSoftObjectPtr<UWorld> CachedMapToUnload;
	TSharedPtr<FStreamableHandle> MapLoadingHandle;
	FTimerHandle ProgressCheckHandle;

	//loading
	bool SceneLoadingRequested = false;
	UPROPERTY()
	TSubclassOf<ULoadingTransition> CurrentLoadingWidgetClass;

	UFUNCTION()
	void HandleLoadingOnMapReady(UWorld* LoadedWorld);
};

