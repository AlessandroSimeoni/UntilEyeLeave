#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Interface/HittableActor.h"
#include "Components/BoxComponent.h"
#include "HittableCollisionBoxComponent.generated.h"

UCLASS(ClassGroup = (Hittable), meta = (BlueprintSpawnableComponent))
class TBO_01_API UHittableCollisionBoxComponent : public UBoxComponent, public IHittableActor
{
	GENERATED_BODY()

public:
	UHittableCollisionBoxComponent();
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity = 1.f) override;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	virtual void BeginPlay() override;

};
