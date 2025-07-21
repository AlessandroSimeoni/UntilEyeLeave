#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Tickable.h"
#include "ChaosCachePlaybackTickable.generated.h"

class AChaosCachePlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChaosCacheTickableFinishedSignature, bool, bReverse);

UCLASS()
class TBO_01_API UChaosCachePlaybackTickable : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UChaosCachePlaybackTickable();

	void Initialize(AChaosCachePlayer* InCachePlayer, float InDuration, bool bPlayReverse, float InPlaybackSpeed);
	void Play();
	void Pause();
	void Resume();
	void Stop();
	void SetDirection(bool bIsReverse);

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;

	UPROPERTY(BlueprintAssignable)
	FChaosCacheTickableFinishedSignature OnFinished;

private:
	TWeakObjectPtr<AChaosCachePlayer> CachePlayer;
	float CurrentTime;
	float Duration;
	float PlaybackSpeed;
	bool bReverse;
	bool bIsPaused;
	bool bIsPlaying;
};
