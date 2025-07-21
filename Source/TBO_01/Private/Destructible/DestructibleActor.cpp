#include "Destructible/DestructibleActor.h"


// Sets default values
ADestructibleActor::ADestructibleActor() 
{
	// Set this actor to call Tick() every frame.  
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CoinContainer = CreateDefaultSubobject<UCoinContainer>("CoinContainer");
	CoinContainer->SetupAttachment(RootComponent);
}



void ADestructibleActor::Destruction()
{
	EffectData.SpawnEffect(GetWorld(),GetActorLocation(),GetActorRotation());
	OnPreDestruction();
}

void ADestructibleActor::CompleteDestruction()
{
	OnDestroyedEvent.Broadcast();
	Destroy();
}