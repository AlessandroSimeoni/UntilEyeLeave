#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

class AMainMenuCameraAggregator;
class AGameCamera;

UCLASS()
class TBO_01_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	ACameraActor* CameraBuffer;
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	float LerpTimeBuffer;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	bool CanLerp = true;
	UPROPERTY(EditDefaultsOnly, Category= "Menu")
	TEnumAsByte<EViewTargetBlendFunction> BlendFunction = VTBlend_EaseInOut;
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	float BlendFunctionExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOptions")
	float FirstLerpTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraOptions")
	float CameraLerpTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOptions")
	float DelayLerpRestart = 0.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void LerpCamera(ACameraActor* Camera, float LerpTime);
	UFUNCTION(BlueprintImplementableEvent, Category = "Menu")
	void LerpCameraComplete(ACameraActor* Camera);
	UFUNCTION()
	void DelayedReset();
	UFUNCTION(BlueprintCallable)
	void ForceCompleteLerp(ACameraActor* OverrideCamera);
protected:
	UPROPERTY(BlueprintReadWrite)
	ACameraActor* InitialCamera = nullptr;
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	ACameraActor* CurrentCamera = nullptr;
	FTimerHandle ExecutionTimerHandle;
	void ResetLerpStatus();
	UPROPERTY()
	AMainMenuCameraAggregator* Aggregator = nullptr;

};
