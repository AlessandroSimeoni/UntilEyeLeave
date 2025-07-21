// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Deathzone.generated.h"

class AMainCharacter;
class UBoxComponent;

UCLASS()
class TBO_01_API ADeathzone : public AActor
{
	GENERATED_BODY()
	
public:
	ADeathzone();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Deathzone", Meta = (UIMin = 0.5f, ClampMin = 0.5f))
	float DeathzoneDamage = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Deathzone")
	bool TeleportToCheckpoint = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Deathzone")
	float InputInterruptionCooldown = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Deathzone|Teleport", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float RayOffset = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Deathzone|Teleport", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float TeleportUpOffset = 30.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Deathzone|Teleport", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float TeleportAwayOffset = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Areas")
	UBoxComponent* DeathArea;
	

	UFUNCTION()
	void OnDeathAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void ResetCollisionResponse() const;
	void EnablePlayerControls() const;
	
	FTimerHandle TriggerAreaTH;
	FTimerHandle InputInterruptionTH;
	FHitResult GroundCheckHit;
	UPROPERTY()
	AMainCharacter* Character;
};
