// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoinContainer.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

class ACoin;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBO_01_API UCoinContainer : public USceneComponent
{
	GENERATED_BODY()

public:
	UCoinContainer();
	virtual void OnComponentCreated() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid UniqueID;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Coin")
	int32 CoinNumber = 0;

	UFUNCTION(BlueprintCallable,Category="Coin")
	void SpawnCoin(const FVector& InstigatorPosition, FVector SpawnLoaction = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable,Category="Coin")
	void SetCoin(int32 value);
	
protected:
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Coin")
	float CoinSpawnInstanceRandomOffsetSphereRadius = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Coin")
	float CoinsSpawnStrenght = 100.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Coin")
	TSubclassOf<ACoin> CoinClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
   TObjectPtr<UNiagaraSystem> CoinNiagaraFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundBase> CoinSpawnSound = nullptr;
    
    UPROPERTY()
    TObjectPtr<UNiagaraComponent> SpawnedNiagaraFX = nullptr;
	
	void CheckCoin();
	
};
