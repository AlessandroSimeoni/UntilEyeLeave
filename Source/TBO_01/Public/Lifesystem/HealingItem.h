// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealingItem.generated.h"

class AMainCharacter;
class USphereComponent;

UCLASS()
class TBO_01_API AHealingItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealingItem();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealingItem", Meta = (UIMin = 0.5f, ClampMin = 0.5f))
	float HealingValue = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trigger Areas")
	USphereComponent* DestroyArea;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trigger Areas")
	USphereComponent* MagneticArea;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealingItem")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Rotation", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float RotationSpeed = 90.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Floating", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float FloatingFrequency = 2.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Floating", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float FloatingDistance = 10.0f; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Magnetism", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float MagneticSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Magnetism|PullAwayHorizontal")
	float MinPullAwayHorizontalDegrees = -20.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Magnetism|PullAwayHorizontal")
	float MaxPullAwayHorizontalDegrees = 20.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Magnetism|PullAwayVertical")
	float MinPullAwayVerticalDegrees = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Magnetism|PullAwayVertical")
	float MaxPullAwayVerticalDegrees = 40.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Movement|Magnetism", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float PullAwayDistance = 5.0f;
	UPROPERTY(BlueprintReadOnly)
	AMainCharacter* Character = nullptr;
	
	bool PlayerInMagneticRange = false;
	float CurrentTime = 0.0f;
	float Period = 0.0f;
	float InitialZLocation = 0.0f;

	UFUNCTION()
	void OnDestroyAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMagneticAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMagneticAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintImplementableEvent, Category = "ItemPickUp")
	void MagneticPickUp(FVector PullAwayLocation);
	UFUNCTION()
	void ActivatePickUp(USceneComponent* LifeComponent = nullptr, float DamageReceived = 0.0f);
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
