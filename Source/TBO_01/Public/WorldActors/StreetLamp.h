// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GameplayObjectState.h"
#include "Interface/HittableActor.h"
#include "StreetLamp.generated.h"

class AB04_Controller;
class AB04_CageBird;
class ACreature;
class UBoxComponent;
class UUniqueIDComponent;

UCLASS()
class TBO_01_API AStreetLamp : public AActor, public IGameplayObjectState, public IHittableActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="StreetLamp")
	void OnStreetLampHit();
	
protected:

	AStreetLamp();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StreetLamp")
	UUniqueIDComponent* UniqueIDComp = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StreetLamp")
	UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StreetLamp")
	USceneComponent* BirdSpawnPoint = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StreetLamp")
	USceneComponent* BirdNestPoint = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StreetLamp")
	TSubclassOf<AB04_CageBird> BirdClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StreetLamp")
	TSubclassOf<AB04_Controller> BirdControllerClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StreetLamp")
	TSubclassOf<ACreature> MouseClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StreetLamp")
	bool HasFallenLantern = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StreetLamp")
	bool LanternStartsActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StreetLamp", Meta = (UIMin = 0, ClampMin = 0))
	int ResistanceLevel = 0;
	
	UPROPERTY(BlueprintReadOnly)
	AB04_CageBird* Bird = nullptr;
	bool Activated = false;

	void BirdFalling();
	UFUNCTION()
	void HandleAltarReached();
	
	virtual bool HasBeenActivated_Implementation() override;
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity = 1.f) override;

	virtual void BeginPlay() override;
};
