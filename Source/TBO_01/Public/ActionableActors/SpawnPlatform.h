// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionableActors/ActionableActor.h"
#include "SpawnPlatform.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class TBO_01_API ASpawnPlatform : public AActionableActor
{
	GENERATED_BODY()

protected:
	ASpawnPlatform();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPlatform")
	bool IsOn = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UStaticMeshComponent> PlatformMeshA = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UStaticMeshComponent> PlatformMeshB = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UStaticMeshComponent> PlatformMeshC = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UStaticMeshComponent> PlatformMeshD = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<USceneComponent> MeshTargetLocationA = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<USceneComponent> MeshTargetLocationB = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<USceneComponent> MeshTargetLocationC = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<USceneComponent> MeshTargetLocationD = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UBoxComponent> PlatformCollision = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "SpawnPlatform")
	bool CurrentlyOn = false;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	FVector StartingLocationMeshA = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	FVector StartingLocationMeshB = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	FVector StartingLocationMeshC = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	FVector StartingLocationMeshD = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	FRotator StartingRotationMeshA = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	FRotator StartingRotationMeshB = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	FRotator StartingRotationMeshC = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	FRotator StartingRotationMeshD = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UMaterialInstanceDynamic> MeshADynMatInstance = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UMaterialInstanceDynamic> MeshBDynMatInstance = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UMaterialInstanceDynamic> MeshCDynMatInstance = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "SpawnPlatform")
	TObjectPtr<UMaterialInstanceDynamic> MeshDDynMatInstance = nullptr;

	UFUNCTION(BlueprintCallable, Category = "SpawnPlatformFunction")
	void InstantStateChange(bool On);
	UFUNCTION(BlueprintCallable, Category = "SpawnPlatformFunction")
	void UpdateCollisionBox(bool On);
	
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void ActivateActor_Implementation(bool InstantActivation = false) override;
	virtual void DeActivateActor_Implementation(bool InstantDeActivation = false) override;
};
