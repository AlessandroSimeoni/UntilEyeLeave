#include "MainMenu/MainMenuCameraAggregator.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

AMainMenuCameraAggregator::AMainMenuCameraAggregator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainMenuCameraAggregator::GetCameras()
{
	MenuCameras.Empty();
	TArray<AActor*> ActorsCollected;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), ActorsCollected);
	for (AActor* Actor : ActorsCollected)
	{
		if (ACameraActor* Item = Cast<ACameraActor>(Actor))
		{
			MenuCameras.Add(Item);
		}
	}
}

ACameraActor* AMainMenuCameraAggregator::GetCameraActorAtIndex(int32 Index)
{
	return MenuCameras[Index];
}


