// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B04_CageBird.h"
#include "Creatures/CreatureAiController.h"
#include "B04_Controller.generated.h"

class AMainCharacter;
/**
 * 
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_FreeMove);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Charged);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Active);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_NestTP);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorCharged, bool, IsAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorFree, bool, IsAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedAltar);
UCLASS()

class TBO_01_API AB04_Controller : public ACreatureAiController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,Category= "B04|Event")
	FOnActorCharged OnActorCharged;
	UPROPERTY(BlueprintAssignable,Category= "B04|Event")
	FOnActorFree  OnActorFree;

	UPROPERTY(BlueprintAssignable,Category= "B04|Event")
	FOnReachedAltar  OnReachedAltar;
	
	UFUNCTION(BlueprintImplementableEvent,Category= "B04|Event")
	void BirdSetActive();
	
	UFUNCTION(BlueprintImplementableEvent,Category= "B04|Event")
	void BirdSetInactive();
	
	//**Used to send the event*/
	UFUNCTION(BlueprintCallable, Category="B04|Event")
	void ActorCharged(const FGameplayTag Tag, int32 NewCount);
	//**Used to send the event*/
	UFUNCTION(BlueprintCallable, Category="B04|Event")
	void ActorFree(const FGameplayTag Tag, int32 NewCount);

	/**Function tp call when altar is reached send the event*/
	UFUNCTION(BlueprintCallable, Category="B04")
	void ReachedAltar();

	/**Set the nest Position is the teleport point when player take damage*/
	UFUNCTION(BlueprintCallable, Category = "B04")
	void AssignNestPosition(FVector Position);

	/**When Altar is assigned move set in blueprint, if set on altar is true teleport the pawn on the altar*/
	UFUNCTION(BlueprintCallable, Category = "B04")
	void AssignAltarPosition(const FTransform& Transform, bool SetOnAltar);
	
	/** When pawn is charged and in free move the pawn is active
	 * While active sight sense is enabled, it can see the player*/
	UFUNCTION(BlueprintCallable, Category = "B04")
	bool TryActivation();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void HandleSight(bool IsSpotted, AActor* Actor) override;

	/** Called when folllowed Player Take Damage*/
	UFUNCTION()
	void OnDamageTakenHandler(USceneComponent* ThisComponent, float Damage);
	void VerifyAndFixOptimizationTick();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameNestPosition = "NestPosition";
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameAltarPosition = "AltarPosition";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "B04")
	FVector NestPosition;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "B04")
	FTransform AltarTransform;

private:
	
	FTimerHandle TimerHandle_VerifyOptimizationTick;
	
	UPROPERTY()
	TObjectPtr<AB04_CageBird> MyCageBirdPawn;

	UPROPERTY()
	TObjectPtr<AMainCharacter> Player;
	
	
};

