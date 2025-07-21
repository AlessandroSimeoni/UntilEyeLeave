// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "SenseOverlapZoneComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBO_01_API USenseOverlapZoneComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	USenseOverlapZoneComponent();

protected:

	/**send damage event with enum TriggerBoxDamage with damage value of 10 */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**send damage event with enum TriggerBoxDamage with damage value of 1
	 *IMPORTANT use the damage value to differentiate the exit */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
