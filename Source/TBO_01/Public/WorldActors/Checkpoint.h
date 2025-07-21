// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Interface/UniqueIDHolder.h"
#include "Checkpoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckpointInteracted, ACheckpoint*, This);

class AMainCharacter;
class UBoxComponent;
enum class EMapIconType : uint8;


UCLASS()
class TBO_01_API ACheckpoint : public AActor, public IInteractable, public IUniqueIDHolder
{
	GENERATED_BODY()
	
public:
	ACheckpoint();

	UPROPERTY(EditAnywhere)
	EMapIconType IconType;
	UPROPERTY(EditAnywhere)
	FText CheckpointName;

	UPROPERTY()
	FOnCheckpointInteracted OnCheckpointInteracted;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Areas")
	UBoxComponent* SaveArea;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Areas")
	UBoxComponent* InteractionArea;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Areas")
	UBoxComponent* HealArea;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Respawn")
	USceneComponent* RespawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* BusStopMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StopSignMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guid")
	UUniqueIDComponent* UniqueIDComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<USceneComponent> InteractionPromptLocation;
	UPROPERTY(BlueprintReadOnly)
	AMainCharacter* Character = nullptr;

	
	UFUNCTION(BlueprintImplementableEvent)
	void AnimateStopSign();
	UFUNCTION()
	void OnSaveAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnHealAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Interact_Implementation() override;
	virtual USceneComponent* GetInteractionPromptSceneComponent_Implementation() override;
	virtual UUniqueIDComponent* GetUniqueIDComponent_Implementation() override;
	FVector GetRespawnLocation();
};
