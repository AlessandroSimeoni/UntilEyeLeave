// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOERing.generated.h"

class UCapsuleComponent;

UCLASS()
class TBO_01_API AAOERing : public AActor
{
	GENERATED_BODY()
	
public:	

	AAOERing();
	virtual void Tick(float DeltaTime) override;

	
	UFUNCTION(BlueprintCallable,Category="AOE")
	void StartRingExpand(float InDestructionLevel,float InDamage,float InMaxSize,float InTimeToReachMax,float KnockabackIntensity = 1.f);
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AOE")
	TObjectPtr<UCapsuleComponent> CapsuleCollision;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AOE")
	float  RingCollisionHeight = 50.0f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AOE")
	float RingCollisionRadius = 50.0f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AOE")
	float CheckInterval = 0.2f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AOE|Visual")
	TObjectPtr<UStaticMeshComponent> RingPlane;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AOE|Visual")
	TObjectPtr<UStaticMeshComponent> RingArea;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AOE|Visual")
	float RingPlaneGrowthFactor = 2.0f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AOE|Visual")
	float RingAreaGrowthFactor = 1.0f;


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	void SetXYScale(UStaticMeshComponent* StaticMesh, float InScaleX, float InScaleY);

	/**Check if the Hitted Actor is in the ring and apply the damage*/
	void CheckForDamage(AActor* Actor);

	/** Apply damage whit Hit Interface*/
	void ApplyDamage(AActor* Actor);

	void CheckDamageOnOverlappingActors();
	
	FTimerHandle AOETickTimerHandle;
	
	UPROPERTY()
	TSet<TObjectPtr<AActor>> OverlappingActors;

	float Damage = 1.f;
	float DestructionLevel = 1.f;
	
	float MaxSize = 1.f;
	float TimeToReachMax = 1.f;
	float CurrentTime = 1.f;
	float RingKnockbackIntensity = 1.f;
	
};

