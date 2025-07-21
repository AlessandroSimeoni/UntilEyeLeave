// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCamera.h"
#include "Engine/TriggerBox.h"
#include "CameraTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API ACameraTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ACameraTriggerBox();

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	float RequiredEnterAngle = 45.0f;
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	float RequiredExitAngle = 45.0f;
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	bool IsEnterDirectionEnabled = false;
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	bool IsExitDirectionEnabled = true;
	
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	bool DrawDebug = false;
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	float DebugLineLength = 400.0f;
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	int32 NumSegments = 32;
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "CameraTrigger")
	void OnCharacterEnter(AGameCamera* GameCamera);
	UFUNCTION(BlueprintImplementableEvent, Category = "CameraTrigger")
	void OnCharacterExit(AGameCamera* GameCamera);
private:
	
	bool IsActorInValidDirection(AActor* Actor, float RequiredAngle) const;
	void DebugAngle(FVector ForwardVector, FVector ToActor, float RequiredAngle) const;
};
