#include "Utility/ChaosCachePlaybackTickable.h"
#include "SceneComponent/ChaosCacheHandler.h"

UChaosCachePlaybackTickable::UChaosCachePlaybackTickable()
    : CurrentTime(0.0f)
    , Duration(0.0f)
    , PlaybackSpeed(1.0f)
    , bReverse(false)
    , bIsPaused(false)
    , bIsPlaying(false)
{
}

void UChaosCachePlaybackTickable::Initialize(AChaosCachePlayer* InCachePlayer, float InDuration, bool bPlayReverse, float InPlaybackSpeed)
{
    CachePlayer = InCachePlayer;
    Duration = FMath::Max(0.0f, InDuration);
    PlaybackSpeed = FMath::Max(0.01f, InPlaybackSpeed);
    bReverse = bPlayReverse;
    CurrentTime = bReverse ? Duration : 0.0f;
}

void UChaosCachePlaybackTickable::Play()
{
    if (!CachePlayer.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("ChaosCachePlayer is invalid!"));
        return;
    }

    CachePlayer->BeginEvaluate();
    CachePlayer->SetStartTime(CurrentTime);
    bIsPaused = false;
    bIsPlaying = true;
}

void UChaosCachePlaybackTickable::Pause()
{
    bIsPaused = true;
}

void UChaosCachePlaybackTickable::Resume()
{
    bIsPaused = false;
}

void UChaosCachePlaybackTickable::Stop()
{
    bIsPaused = true;
    bIsPlaying = false;
    CurrentTime = bReverse ? Duration : 0.0f;
    if (CachePlayer.IsValid())
    {
        CachePlayer->SetStartTime(CurrentTime);
        CachePlayer->EndEvaluate();
    }
}

void UChaosCachePlaybackTickable::SetDirection(bool bIsReverse)
{
    bReverse = bIsReverse;
}

void UChaosCachePlaybackTickable::Tick(float DeltaTime)
{
    if (!bIsPlaying || bIsPaused || !CachePlayer.IsValid() || Duration <= 0.0f)
    {
        return;
    }

    float Step = DeltaTime * PlaybackSpeed;

    if (bReverse)
    {
        CurrentTime -= Step;
        if (CurrentTime <= 0.0f)
        {
            CurrentTime = 0.0f;
            CachePlayer->SetStartTime(CurrentTime);
            CachePlayer->EndEvaluate();
            bIsPlaying = false;  // Stop tick
            OnFinished.Broadcast(true);
            return;
        }
    }
    else
    {
        CurrentTime += Step;
        if (CurrentTime >= Duration)
        {
            CurrentTime = Duration;
            CachePlayer->SetStartTime(CurrentTime);
            CachePlayer->EndEvaluate();
            bIsPlaying = false;  // Stop tick
            OnFinished.Broadcast(false);
            return;
        }
    }

    CachePlayer->SetStartTime(CurrentTime);
}

TStatId UChaosCachePlaybackTickable::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UChaosCachePlaybackTickable, STATGROUP_Tickables);
}

bool UChaosCachePlaybackTickable::IsTickable() const
{
    return bIsPlaying && !bIsPaused;
}
