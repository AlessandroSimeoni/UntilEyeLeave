#include "SceneComponent/ChaosCacheHandler.h"
#include "Chaos/CacheCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

UChaosCacheHandler::UChaosCacheHandler()
{
    PrimaryComponentTick.bCanEverTick = true;
   
}
void UChaosCacheHandler::BeginPlay()
{
    Super::BeginPlay();
    SetComponentTickEnabled(false);
}

void UChaosCacheHandler::OnUnregister()
{
    Super::OnUnregister();
   // UE_LOG(LogTemp, Warning, TEXT("component Destroyed"));
    if (IsValid(ChaosCachePlayer))
    {
        ChaosCachePlayer->Destroy();
    }
}

void UChaosCacheHandler::InitializeCollection(UGeometryCollectionComponent* CollectionComponent, UChaosCacheCollection* MyCacheCollection)
{
    check(CollectionComponent);
    check(MyCacheCollection);

    GeometryCollectionComponent = CollectionComponent;
    ChaosCacheCollection = MyCacheCollection;
   
    if (!ChaosCachePlayer)
    {
        FTransform ComponentWorldTransform = CollectionComponent->GetComponentTransform();
        ChaosCachePlayer = CreateAndSetupCacheManager(ComponentWorldTransform);
    }

    Duration = ChaosCacheCollection->Caches[0]->GetDuration();
    CurrentTime = bReverse ? Duration : 0.0f;

    
    ChaosCachePlayer->SetStartTime(CurrentTime);
    ChaosCachePlayer->BeginEvaluate();
    ChaosCachePlayer->EndEvaluate();

    if (!bKeepChaosCacheManagerAlive)
    {
        ChaosCachePlayer->Destroy();
        ChaosCachePlayer = nullptr;
        UE_LOG(LogTemp, Log, TEXT("ChaosCacheManager destroyed immediately (KeepAlive disabled)"));
    }
}
AChaosCachePlayer* UChaosCacheHandler::CreateAndSetupCacheManager(const FTransform& ComponentWorldTransform)
{
    AChaosCachePlayer* NewManager = GetWorld()->SpawnActor<AChaosCachePlayer>(AChaosCachePlayer::StaticClass(), ComponentWorldTransform, FActorSpawnParameters());


    if (!NewManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn ChaosCacheManager!"));
        return nullptr;
    }
    
    NewManager->SetActorHiddenInGame(bManagerHiddeInGame);

    NewManager->CacheCollection = ChaosCacheCollection.Get();
    NewManager->CacheMode = ECacheMode::None;
    NewManager->StartMode = EStartMode::Timed;

    FObservedComponent NewObservedComponent;
    NewObservedComponent.SoftComponentRef.OtherActor = GeometryCollectionComponent->GetOwner();
    NewObservedComponent.SoftComponentRef.ComponentProperty = GeometryCollectionComponent->GetFName();

    if (ChaosCacheCollection->Caches.Num() > 0)
    {
        NewObservedComponent.CacheName = ChaosCacheCollection->Caches[0]->GetFName();
    }
    else
    {
        NewObservedComponent.CacheName = FName("DefaultCache");
    }

    NewObservedComponent.bPlaybackEnabled = true;
    NewObservedComponent.bIsSimulating = true;
    NewObservedComponent.ResetRuntimeData(EStartMode::Timed);

    NewManager->GetObservedComponents().Add(NewObservedComponent);

    return NewManager;
}

void UChaosCacheHandler::Play(float Speed)
{
    if (!IsValid(ChaosCachePlayer))
    {
        FTransform ComponentWorldTransform = GeometryCollectionComponent->GetComponentTransform();
        ChaosCachePlayer = CreateAndSetupCacheManager(ComponentWorldTransform);
    }

    if (!ChaosCachePlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("ChaosCachePlayer is not valid!"));
        return;
    }

    
    PlaybackSpeed = FMath::Max(0.01f, Speed);

    ChaosCachePlayer->BeginEvaluate();
    ChaosCachePlayer->SetStartTime(CurrentTime);

    bIsPlaying = true;
    bIsPaused = false;

    SetComponentTickEnabled(true);
    UE_LOG(LogTemp, Log, TEXT("Playback started with speed: %f"), PlaybackSpeed);
}

void UChaosCacheHandler::SetReverseAndPlay(bool bInReverse, float Speed)
{
    bReverse = bInReverse;
    Play(Speed);
}

void UChaosCacheHandler::Stop()
{
    bIsPlaying = false;
    bIsPaused = true;
    SetComponentTickEnabled(false);

    if (IsValid(ChaosCachePlayer))
    {
        ChaosCachePlayer->EndEvaluate();
    }
}

void UChaosCacheHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    

    float Step = DeltaTime * PlaybackSpeed;

    //UE_LOG(LogTemp, Log, TEXT("Tick Component Chaos"));
    if (bReverse)
    {
        CurrentTime -= Step;
        if (CurrentTime <= 0.0f)
        {
            CurrentTime = 0.0f;
            ChaosCachePlayer->SetStartTime(CurrentTime);
            ChaosCachePlayer->EndEvaluate();
            HandlePlaybackFinished(true);
            return;
        }
    }
    else
    {
        CurrentTime += Step;
        if (CurrentTime >= Duration)
        {
            CurrentTime = Duration;
            ChaosCachePlayer->SetStartTime(CurrentTime);
            ChaosCachePlayer->EndEvaluate();
            HandlePlaybackFinished(false);
            return;
        }
    }

    ChaosCachePlayer->SetStartTime(CurrentTime);
}



void UChaosCacheHandler::HandlePlaybackFinished(bool bReachedReverseEnd)
{
    bIsPlaying = false;
    SetComponentTickEnabled(false);

    OnChaosReachEnd.Broadcast(bReachedReverseEnd);

    if (bHideCollectionOnEnd && IsValid(GeometryCollectionComponent))
    {
        GeometryCollectionComponent->SetVisibility(false);
    }

    if (!bKeepChaosCacheManagerAlive && IsValid(ChaosCachePlayer))
    {
        ChaosCachePlayer->Destroy();
        ChaosCachePlayer = nullptr;
    }
}
