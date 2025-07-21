#include "SceneComponent/HittableCollisionBoxComponent.h"
#include "Components/BoxComponent.h"

UHittableCollisionBoxComponent::UHittableCollisionBoxComponent()
{
   
}

bool UHittableCollisionBoxComponent::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity)
{
   //UE_LOG(LogTemp, Warning, TEXT("HitzoneAreaHitted"));
   return IHittableActor::Hit_Implementation(CharacterLevel, DamageValue, AttackInstigator,Intensity);
   
}

#if WITH_EDITOR
void UHittableCollisionBoxComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
   Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
void UHittableCollisionBoxComponent::BeginPlay()
{
   Super::BeginPlay();
}
