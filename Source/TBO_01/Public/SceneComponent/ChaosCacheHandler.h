#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Chaos/CacheManagerActor.h"
#include "ChaosCacheHandler.generated.h"

class UGeometryCollectionComponent;
class UChaosCacheCollection;
class AChaosCachePlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChaosChaosReachEnd, bool, bReverse);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBO_01_API UChaosCacheHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	UChaosCacheHandler();

	UPROPERTY(BlueprintAssignable)
	FChaosChaosReachEnd OnChaosReachEnd;

	UFUNCTION(BlueprintCallable)
	void InitializeCollection(UGeometryCollectionComponent* CollectionComponent, UChaosCacheCollection* MyCacheCollection);

	UFUNCTION(BlueprintCallable)
	void Play(float Speed = 1.0f);

	UFUNCTION(BlueprintCallable)
	void SetReverseAndPlay(bool bInReverse, float Speed = 1.0f);

	UFUNCTION(BlueprintCallable)
	void Stop();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void OnUnregister() override;

private:
	AChaosCachePlayer* CreateAndSetupCacheManager(const FTransform& ComponentWorldTransform);

	void HandlePlaybackFinished(bool bReverse);

	UPROPERTY()
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;

	UPROPERTY()
	TObjectPtr<UChaosCacheCollection> ChaosCacheCollection;

	UPROPERTY()
	TObjectPtr<AChaosCachePlayer> ChaosCachePlayer;

	bool bIsPlaying = false;
	bool bIsPaused = false;
	
	UPROPERTY(EditAnywhere, Category = "Chaos",meta=(AllowPrivateAccess=true))
	bool bReverse = false;

	float CurrentTime = 0.0f;
	float Duration = 0.0f;
	float PlaybackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Chaos",meta=(AllowPrivateAccess=true))
	bool bKeepChaosCacheManagerAlive = false;

	UPROPERTY(EditAnywhere, Category = "Chaos",meta=(AllowPrivateAccess=true))
	bool bHideCollectionOnEnd = false;

	UPROPERTY(EditAnywhere, Category = "Chaos",meta=(AllowPrivateAccess=true))
	bool bManagerHiddeInGame = true;
};
