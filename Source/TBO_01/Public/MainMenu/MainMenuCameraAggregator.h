#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuCameraAggregator.generated.h"

UCLASS()
class TBO_01_API AMainMenuCameraAggregator : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AMainMenuCameraAggregator();
	UPROPERTY(EditAnywhere)
	TArray<ACameraActor*> MenuCameras;

	UFUNCTION(CallInEditor, Category = "Camera")
	void GetCameras();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACameraActor* GetCameraActorAtIndex(int32 Index);
};
