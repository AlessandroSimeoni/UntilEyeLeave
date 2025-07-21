// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/ActorComponent.h"
#include "EyeSceneComponent.h"
#include "NativeGameplayTags.h"
#include "EyesHandlerActorComponent.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ClosedEyes);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnEyesHit, UEyeSceneComponent*, Eye, float, Damage, const AActor*, ActorInstigator, float, Force);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEyeDeath, UEyeSceneComponent*, Eye,const AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAllEyesDeath, UEyeSceneComponent*, Eye,const AActor*, Instigator);

UCLASS( ClassGroup=(Eyes), meta=(BlueprintSpawnableComponent) )
class TBO_01_API UEyesHandlerActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	float EyesTransitionTime = 1;

	
	
	// Eventi Multicast
	UPROPERTY(BlueprintAssignable, Category="Eyes")
	FOnEyesHit OnEyesHit;

	UPROPERTY(BlueprintAssignable, Category="Eyes")
	FOnEyeDeath OnEyeDeath;

	UPROPERTY(BlueprintAssignable, Category="Eyes")
	FOnAllEyesDeath OnAllEyesDeath;
	
	UEyesHandlerActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**Register to eyes event and set up skeletalmesh for morph, change parameter if !UseEyesDefaultsValue*/
	UFUNCTION(BlueprintCallable, Category = "Eye")
	void SetUpEyes(TArray<UEyeSceneComponent*> EyeSceneComponents,float EyesHitPoint,float EyesLevel,float TransitionTime,USkeletalMeshComponent* SkeletalMeshComponent);

	UFUNCTION(BlueprintCallable, Category = "Eye")
	void SetEyesState(bool IsOpen, bool Instant);
	UFUNCTION(BlueprintCallable, Category = "Eye")
	void SetEyesTargetPlayer(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Eye")
	void RemoveEyesTargetPlayer(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Eye")
	void RestoreEyes();

	void SetOptimizationTick(bool bIsInRange);

protected:
	virtual void BeginPlay() override;
	/**if set true use the eyes Level and Hitpoint value*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Eye")
	bool UseEyesComponentValue = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eyes")
	bool EyesLidOpen = true;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Eyes")
	TObjectPtr<USkeletalMeshComponent> EyelidMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Eyes")
	FName MeshMorphName;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Eye")
	TArray<TObjectPtr<UEyeSceneComponent>> Eyes;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	

	
	UFUNCTION()
	void HandleEyeHit(UEyeSceneComponent* Eye,float Damage,const AActor* ActorInstigator,float Force);
	UFUNCTION()
	void HandleEyeDeath(UEyeSceneComponent* Eye, const AActor* LastInstigator);
	bool CheckAllEyesDeath();

	/**Return the percentage of remaining living eyes */
	float LivingEyesRemaining();
	

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	bool bOptimizeTick = true;
	float CurrentEyesTransitionAlpha = 1.0f;
	/**  target (0 Open Eye, 1 Close Eye)*/
	float TargetEyesTransitionAlpha = 0.0f;  
	bool bIsTransitioning = false;         
	float DynamicTransitionTime = 1.0f;  
		
};

inline void UEyesHandlerActorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}
