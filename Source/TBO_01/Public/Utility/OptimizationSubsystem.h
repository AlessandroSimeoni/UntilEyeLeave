// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "OptimizationSubsystem.generated.h"

class UEyesHandlerActorComponent;
class ACreatureAiController;
class ACreature;
class AHittableDestructibleActor;
/**
 * 
 */
UCLASS()
class TBO_01_API UOptimizationSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	 static UOptimizationSubsystem* Get(const UWorld* World)
	 {
		 if (World)
		 {
			 return World->GetSubsystem<UOptimizationSubsystem>();
		 }
	 	return nullptr;
	 }
protected:
	TArray<TWeakObjectPtr<AHittableDestructibleActor>> HittableDestructibles;
	TArray<TWeakObjectPtr<ACreature>> Creatures;
	TArray<TWeakObjectPtr<UEyesHandlerActorComponent>> EyesHandlerActorComponents;
	//TArray<TWeakObjectPtr<ACreatureAiController>> CreatureAiControllers;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
	void AddHittableDestructibleActor(AHittableDestructibleActor* HittableDestructibleActor);
	void RemoveHittableDestructibleActor(AHittableDestructibleActor* HittableDestructibleActor);
	
	void AddCreature(ACreature* Creature);
	void RemoveCreature(ACreature* Creature);
	
	void AddEyesHandlerActorComponent(UEyesHandlerActorComponent* Handler);
	void RemoveEyesHandlerActorComponent(UEyesHandlerActorComponent* Handler);

	

	

	
private:
	bool bTickDetstructible = false;
	
	template<typename T>
void AddToList(TArray<TWeakObjectPtr<T>>& List, T* Object)
	{
		if (Object)
		{
			List.AddUnique(MakeWeakObjectPtr(Object));
		}
	}

	template<typename T>
	void RemoveFromList(TArray<TWeakObjectPtr<T>>& List, T* Object)
	{
		if (Object)
		{
			List.RemoveSingleSwap(MakeWeakObjectPtr(Object));
		}
	}
};

