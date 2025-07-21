// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SightFieldOfView.h"
#include "ComplexCreatureSight.generated.h"


DECLARE_DELEGATE_TwoParams(FOnTargetSpottedChanged, bool, FActorSpotted);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TBO_01_API UComplexCreatureSight : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComplexCreatureSight();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//Function
	UFUNCTION(BlueprintCallable,Category = "ComplexCreatureSight")
	void AddSpottedActor(AActor* Actor);
	UFUNCTION(BlueprintCallable,Category = "ComplexCreatureSight")
	void RemoveSpottedActor(AActor* Actor);
	UFUNCTION(BlueprintCallable,Category = "ComplexCreatureSight")
	void SetPawn(APawn* ControlledPawn);
	
	UFUNCTION()
	void RefreshSpottedActor(AActor *Actor);
	

	
	
	
	//Delegate to inform the change of the spotted status
	FOnTargetSpottedChanged OnTargetSpottedChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	
	//property
	/** Sense Property */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ComplexCreatureSight")
	TArray<FSightFieldOfView> FieldOfViews;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="ComplexCreatureSight")
	TArray<FActorSpotted> SpottedActors;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ComplexCreatureSight")
	bool DrawDebug = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ComplexCreatureSight")
	float LooseSightTime = 3.0f;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APawn> Pawn;
	
	
private:
	
	int8 CurrentPositionInCheck = 0;

	/** return true if actor is inside the line of sight */
	bool CheckActorSpotted(FSightFieldOfView View,FActorSpotted ActorToCheck);
	/** change the spotted status and notify in case of a change */
	void SetTargetSpotted(bool IsSpotted, FActorSpotted &SpottedActor);
	/**Confirm the remove with timer*/
	void ConfirmRemoveActor(FActorSpotted& ActorSpotted);
	/**Debug cone*/
	void DrawDebugVisionCone();
	void CleanupInvalidActors();
};

