// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EyesHandlerInterface.h"
#include "GameFramework/Actor.h"
#include "EyesHolderActor.generated.h"

UCLASS()
class TBO_01_API AEyesHolderActor : public AActor, public IEyesHandlerInterface
{
	GENERATED_BODY()
	
public:	
	AEyesHolderActor();


	//eyes event 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Eye")
	void OnEyeHit(UEyeSceneComponent* Eye, float Damage, const AActor* ActorInstigator, float Force);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Eye")
	void OnEyeDeath(UEyeSceneComponent* Eye, const AActor* ActorInstigator);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Eye")
	void OnAllEyesDeath(UEyeSceneComponent* Eye, const AActor* ActorInstigator);

	
	UFUNCTION(BlueprintCallable,Category="Eye")
	virtual UEyesHandlerActorComponent* GetEyesHandlerActorComponent_Implementation() override;

	

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eye")
	TObjectPtr<UEyesHandlerActorComponent> EyesHandlerActorComponent;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Eye")
	TObjectPtr<USkeletalMeshComponent> EyesBody;
	
	UPROPERTY()
	TArray<TObjectPtr<UEyeSceneComponent>> Eyes;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eye")
	int EyesHitPoint = 1;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eye")
	int EyesLevel = 1;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eye")
	float EyesTransition = 0.2f;
};
