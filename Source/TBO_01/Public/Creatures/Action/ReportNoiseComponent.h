// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReportNoiseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent,OnlyPlaceable) )
class TBO_01_API UReportNoiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReportNoiseComponent();
	
	UFUNCTION(BlueprintCallable, Category="Action|Noise")
	void MakeNoiseDefault();
	UFUNCTION(BlueprintCallable, Category="Action|Noise")
	void MakeNoise(ENoiseAction SoundTypeValue,float LoudnessValue,float MaxRangeValue);
private:
	UPROPERTY(EditAnywhere, Category="Action|Noise")
	ENoiseAction SoundType;
	UPROPERTY(EditAnywhere, Category="Action|Noise")
	float Loudness = 1.0f;
	//** 0 is no limit*/
	UPROPERTY(EditAnywhere, Category="Action|Noise")
	float MaxRange = 5000;
	UPROPERTY(EditAnywhere, Category="Action|Noise")
	bool DrawDebug = false;
	void DrawNoiseDebug(FVector Location, float Radius);
};
