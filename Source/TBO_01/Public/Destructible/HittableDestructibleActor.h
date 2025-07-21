// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Destructible/DestructibleActor.h"
#include "HittableDestructibleActor.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API AHittableDestructibleActor : public ADestructibleActor, public IHittableActor
{
	GENERATED_BODY()

public:
	
	
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator,float Intensity = 1.f) override;
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() { return SkeletalMeshComponent.Get(); }
	
	

protected:
	TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;


	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	int ResistanceLevel = 0;
};
