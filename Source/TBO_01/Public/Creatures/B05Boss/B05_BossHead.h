// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/ShooterCreature.h"
#include "B05_BossHead.generated.h"

class AAIController;
class ACreatureAiController;
class ABossAiControllor;
class AB05_BossHand;

UCLASS()
class TBO_01_API AB05_BossHead : public AShooterCreature
{
	GENERATED_BODY()

public:
	AB05_BossHead();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly,Category="Hand")
	TWeakObjectPtr<AB05_BossHand> DestructibleHand;
	UPROPERTY(BlueprintReadOnly,Category="Hand")
	TWeakObjectPtr<AB05_BossHand> StepOnHand;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Hand")
	TObjectPtr<AActor> DestructibleHandRestPoint;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Hand")
	TObjectPtr<AActor>StepOnHandRestPoint;
	
	UFUNCTION(BlueprintCallable,Category="Hand")
	ACreature*  SpawnDestructibleHand();
	UFUNCTION(BlueprintCallable,Category="Hand")
	ACreature*  SpawnStepOnHand();

	/**Spawn a creature in the specified position posses the controller
	 * return a pointer to the creature 
	 */
	UFUNCTION(BlueprintCallable,Category="Minion")
	ACreature* SpawnMinion(FTransform TrasnformPosition,TSubclassOf<ACreature> MinionClass,TSubclassOf<AAIController> MinionControllerClass);

	UFUNCTION(BlueprintCallable,Category="Eye")
	void SetEyesActive();
	UFUNCTION(BlueprintCallable,Category="Hand")
	void DestroyHands();
	UFUNCTION(BlueprintCallable,Category="Hand")
	void SetHandsToRest();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Hand")
	TSubclassOf<AB05_BossHand> DestructibleHandClass;
	UPROPERTY(EditDefaultsOnly,Category="Hand")
	TSubclassOf<ABossAiControllor> DestructibleHandControllerClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Hand")
	TObjectPtr<AActor> DestructibleHandSpawnPosition;

	UPROPERTY(EditDefaultsOnly,Category="Hand")
	TSubclassOf<AB05_BossHand> StepOnHandClass;
	UPROPERTY(EditDefaultsOnly,Category="Hand")
	TSubclassOf<AAIController> StepOnHandControllerClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Hand")
	TObjectPtr<AActor>  StepOnHandSpawnPosition;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Hand")
	TObjectPtr<APatrolPath> StepOnHandPatrolPath;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eye")
	TObjectPtr<USkeletalMeshComponent> LeftEye ;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eye")
	TObjectPtr<USkeletalMeshComponent> RightEye;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName LeftEyeSocketName ="Eye_LXSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName RightEyeSocketName = "Eye_RXSocket";
};
