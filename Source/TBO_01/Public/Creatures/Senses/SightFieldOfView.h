
#pragma once

#include "SightFieldOfView.generated.h"

USTRUCT(BlueprintType)
struct FSightFieldOfView
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SightFieldOfView")
	float DegreeAngle;
	UPROPERTY(BlueprintReadOnly,category="SightFieldOfView")
	float RadiansAngle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SightFieldOfView")
	float Distance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SightFieldOfView")
	float LoseDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SightFieldOfView")
	FColor DebugColor = FColor::Green;

	//Constructor for initialization
	FSightFieldOfView()
	: DegreeAngle(90.0f)
	, RadiansAngle(0.0f)   
	, Distance(1000.0f)
	, LoseDistance(1200.0f)
	, DebugColor(FColor::Green) 
{}
};

USTRUCT(BlueprintType)
struct FActorSpotted
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="SightFieldOfView")
	TWeakObjectPtr<AActor> Actor; 
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="SightFieldOfView")
	bool IsSpotted;
	UPROPERTY()
	FTimerHandle TimerHandleExitCone;
	UPROPERTY()
	FTimerHandle TimerHandleExitLineSightSense;
	
	//Constructor for initialization
	FActorSpotted(): Actor(nullptr) ,IsSpotted(false) {}
};
