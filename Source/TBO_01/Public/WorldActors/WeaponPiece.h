// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Journal/CollectableItems.h"
#include "WeaponPiece.generated.h"

class AActionableActor;

UCLASS()
class TBO_01_API AWeaponPiece : public ACollectableItems
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connected Actionable Actors")
	TArray<TObjectPtr<AActionableActor>> ConnectedActionableActors;
	
	UFUNCTION(BlueprintImplementableEvent)
	void WeaponTorchNotify();
	
	virtual void HandleCollectedStatus() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void ActionActors(const bool InstantActivation = false);
};
