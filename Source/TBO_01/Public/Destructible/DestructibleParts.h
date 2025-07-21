// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneComponent/HittableCollisionBoxComponent.h"
#include "Utility/EffectData.h"
#include "DestructibleParts.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreDestructionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestructionCompleteDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TBO_01_API UDestructibleParts : public UHittableCollisionBoxComponent
{
	GENERATED_BODY()

public:	
	UDestructibleParts();

	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity = 1.f) override;

	UFUNCTION(BlueprintCallable, Category = "Destruction")
	void RegisterComponent(USceneComponent* ComponentToHide);

	UFUNCTION(BlueprintCallable, Category = "Destruction")
	void StartDestruction();

	UFUNCTION(BlueprintCallable, Category = "Destruction")
	void CompleteDestruction();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	int ResistanceLevel = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "Destruction")
	TArray<TObjectPtr<USceneComponent>> RegisteredComponents;

	UPROPERTY(EditAnywhere, Category = "Destruction")
	FEffectData EffectData;

	UPROPERTY(BlueprintAssignable, Category = "Destruction")
	FOnPreDestructionDelegate OnPreDestruction;

	UPROPERTY(BlueprintAssignable, Category = "Destruction")
	FOnDestructionCompleteDelegate OnDestructionComplete;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	virtual void BeginPlay() override;
};