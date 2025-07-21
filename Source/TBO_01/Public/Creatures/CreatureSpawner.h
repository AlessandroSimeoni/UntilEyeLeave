// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreatureSpawner.generated.h"

class USaveSubsystem;
class UUniqueIDComponent;
class UNiagaraComponent;
class IGameplayObjectState;
class ACreature;

UCLASS()
class TBO_01_API ACreatureSpawner : public AActor
{
	GENERATED_BODY()

	ACreatureSpawner();
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
	UUniqueIDComponent* UniqueIDComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TObjectPtr<ACreature> Creature = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner", Meta = (UIMin = 0, ClampMin = 0))
	float SpawnDelay = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TArray<AActor*> DependantGameplayObjects;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
	UNiagaraComponent* SpawnVFX = nullptr;
	
	UPROPERTY()
	TSubclassOf<ACreature> CreatureClass;
	UPROPERTY()
	USaveSubsystem* SaveSubsystem = nullptr;
	UPROPERTY()
	TArray<TScriptInterface<IGameplayObjectState>> DependantGameplayObjectsConverted;
	
	UFUNCTION()
	void HandleCreatureDeath(AActor* TargetCreature, const AActor* DeathInstigator);
	UFUNCTION()
	void RespawnCreature();
	UFUNCTION()
	void InitialSetup();
	bool PendingDependantObjects();

	FTimerHandle TH;
	FTimerHandle DelayTH;
};
