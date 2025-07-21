// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Currency/CoinContainer.h"
#include "GameFramework/Actor.h"
#include "Interface/HittableActor.h"
#include "Utility/EffectData.h"
#include "DestructibleActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyedEvent);
UCLASS()
class TBO_01_API ADestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADestructibleActor();



protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
	TObjectPtr<UCoinContainer> CoinContainer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction")
	FEffectData EffectData;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Destruction")
	void OnPreDestruction();

	
	UFUNCTION(BlueprintCallable, Category = "Destruction")
	void CompleteDestruction();
	
	
	UFUNCTION()
	void Destruction();


	UPROPERTY(BlueprintAssignable, Category = "Destruction")
	FOnDestroyedEvent OnDestroyedEvent;
};