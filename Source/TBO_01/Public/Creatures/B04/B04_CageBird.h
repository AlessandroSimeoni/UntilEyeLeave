// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/Creature.h"
#include "B04_CageBird.generated.h"

/**
 * 
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Charge);

UCLASS()
class TBO_01_API AB04_CageBird : public ACreature
{
	GENERATED_BODY()

public:


	//exposed function for charge
	UFUNCTION(BlueprintCallable, Category="B04|Ability")
	void SelfCharge();

	//exposed function for charge
	UFUNCTION(BlueprintCallable, Category="B04|Ability")
	void SetFree(bool IsFree);
	
	
	/**set the movement mode in flight at the Begin play if true*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="B04|Status")
	bool StartInFlight;
	/**Us The Charge Gameplay ability*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="B04|Status")
	bool StartCharged;
	/**Start free if is charge the behavior tree is active can moce*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="B04|Status")
	bool StartFree;


	
protected:
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	virtual void BeginPlay() override;
private:
	void UseAbilitySelfCharge();
	

	
};
