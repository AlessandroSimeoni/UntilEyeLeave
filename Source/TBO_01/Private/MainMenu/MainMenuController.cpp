#include "MainMenu/MainMenuController.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenu/MainMenuCameraAggregator.h"

void AMainMenuController::LerpCamera(ACameraActor* Camera, float LerpTime)
{
	AActor* CurrentTarget = GetViewTarget();
	if (CurrentTarget != Camera)
	{
		if (!CanLerp)
		{
			CameraBuffer = Camera;
			LerpTimeBuffer = LerpTime;
			return;
		}
		CurrentCamera = Camera;
		SetViewTargetWithBlend(Camera, LerpTime, BlendFunction, BlendFunctionExp);
		CanLerp = false;
		GetWorld() -> GetTimerManager().SetTimer(ExecutionTimerHandle, this, &AMainMenuController::ResetLerpStatus, LerpTime, false);
	}
}

void AMainMenuController::ResetLerpStatus()
{
	LerpCameraComplete(InitialCamera);
	ExecutionTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(ExecutionTimerHandle, this, &AMainMenuController::DelayedReset, DelayLerpRestart, false);
	
}
void AMainMenuController::DelayedReset()
{
	ExecutionTimerHandle.Invalidate();
	CanLerp = true;
	InitialCamera = nullptr;
	if (CameraBuffer != nullptr)
	{
		LerpCamera(CameraBuffer, LerpTimeBuffer);
		CameraBuffer = nullptr;
		LerpTimeBuffer = -1.f;
	}
}

void AMainMenuController::ForceCompleteLerp(ACameraActor* OverrideCamera)
{
	SetViewTarget(Aggregator ->GetCameraActorAtIndex(0));
	SetViewTarget(OverrideCamera);
	CameraBuffer = nullptr;
	LerpTimeBuffer = -1.f;
	CurrentCamera = nullptr;
	InitialCamera = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(ExecutionTimerHandle);
	ExecutionTimerHandle.Invalidate();
	CanLerp = true;
}

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainMenuCameraAggregator::StaticClass(), FoundActors);
	Aggregator = Cast<AMainMenuCameraAggregator>(FoundActors[0]);
}

