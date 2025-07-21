#include "Journal/JournalManager.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Journal/CollectableItems.h"
#include "Journal/Fragment.h"
#include "Journal/IconTableRow.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"
#include "SaveSystem/UniqueIDComponent.h"
#include "WorldActors/Checkpoint.h"

AJournalManager::AJournalManager()
{
	PrimaryActorTick.bCanEverTick = false;
	//SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
}

void AJournalManager::GetAllCollectibles()
{
	CollectableItems.Empty();
	TArray<AActor*> ActorsCollected;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectableItems::StaticClass(), ActorsCollected);
	for (AActor* Actor : ActorsCollected)
	{
		if (ACollectableItems* Item = Cast<ACollectableItems>(Actor))
		{
			CollectableItems.Add(Item);
		}
	}
}

void AJournalManager::GetAllCheckpoints()
{
	Checkpoints.Empty();
	TArray<AActor*> ActorsCollected;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), ActorsCollected);
	for (AActor* Actor : ActorsCollected)
	{
		if (ACheckpoint* Checkpoint = Cast<ACheckpoint>(Actor))
		{
			Checkpoints.Add(Checkpoint);
		}
	}
}

void AJournalManager::CaptureMap()
{
	if (!UpperLeft || !LowerRight /*|| !RenderTarget*/) return;

	FVector UpperLeftLocation = UpperLeft->GetActorLocation();
	FVector LowerRightLocation = LowerRight->GetActorLocation();

	MapBounds.Max = FVector2D(UpperLeftLocation.X, UpperLeftLocation.Y);
	MapBounds.Min = FVector2D(LowerRightLocation.X, LowerRightLocation.Y);

	/*FVector CenterLocation = (UpperLeftLocation + LowerRightLocation) * 0.5f;

	float Width = FMath::Abs(LowerRightLocation.Y - UpperLeftLocation.Y);
	float Height = FMath::Abs(LowerRightLocation.X - UpperLeftLocation.X);

	FVector CaptureLocation = FVector(CenterLocation.X, CenterLocation.Y, CenterLocation.Z + 3000.0f);
	SceneCapture->SetWorldLocation(CaptureLocation);

	FRotator CaptureRotation = FRotator(-90.0f, 0.0f, 0.0f);
	SceneCapture->SetWorldRotation(CaptureRotation);

	SceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCapture->OrthoWidth = FMath::Max(Width, Height);
	SceneCapture->TextureTarget = RenderTarget;
	SceneCapture->CaptureScene();*/
}

FMapBounds AJournalManager::GetMapBounds() const
{
	return MapBounds;
}

FVector2D AJournalManager::ConvertWorldToMapLocation(const FVector& WorldLocation, const FMapBounds& Bounds, const FVector2D& MapImageSize)
{
	FVector2D World2D(WorldLocation.X, WorldLocation.Y);
	FVector2D BoundsSize = FVector2D(
	Bounds.Max.X - Bounds.Min.X,
	Bounds.Min.Y - Bounds.Max.Y 
);

	FVector2D Normalized = FVector2D(
	(World2D.X - Bounds.Min.X) / BoundsSize.X,
	(Bounds.Min.Y - World2D.Y) / BoundsSize.Y 
);
	float AspectRatioCorrection = BoundsSize.X / BoundsSize.Y;

	Normalized.Y *= AspectRatioCorrection;
	
	Normalized /= FVector2D(1.0f, AspectRatioCorrection);
	Normalized.X = 1.0f - Normalized.X;
	Normalized.Y = 1.0f - Normalized.Y;
	FVector2D MapPosition = Normalized * MapImageSize;
	
	return FVector2D(MapPosition.Y, MapPosition.X);

}

UTexture2D* AJournalManager::GetIconFromDataTable(EMapIconType IconType, bool Collected)
{
	FName Name = UEnum::GetValueAsName(IconType);
	FIconTableRow* IconRow = IconData->FindRow<FIconTableRow>(Name, "SearchRow");
	if (IconRow && !Collected)
		return IconRow->EntryImage;
	if (IconRow && Collected)
		return IconRow->CollectedImage;
	return nullptr;
}

void AJournalManager::UpdateAvailableCheckpoints(ACheckpoint* Checkpoint)
{
	AvailableCheckpoints.AddUnique(Checkpoint);
}

void AJournalManager::CollectFragment(FName EntryID, EPageType Section, TArray<int32> Indexes, bool IsTrigger)
{
	IndexToOpen = static_cast<int32>(Section);
	if (Section == EPageType::Map)
	{
		SaveGame -> AddMapFragment(Indexes);
	}
	if (!SaveGame ->PersistentJournalEntries.Contains(EntryID))
	{
		SaveGame ->AddJournalEntry(EntryID);
		//NewEntry.FragmentsCollected.Append(Indexes);
	}
	SaveGame -> AppendJournalEntryIndexes(EntryID, Indexes);
	
	OnFragmentCollected.Broadcast(EntryID, Section, IsTrigger);
}



void AJournalManager::UpdateCollectable(ACollectableItems* Actor)
{
	//Actor->OnObjectCollected.RemoveAll(this);
	AvailableCollectibles.Remove(Actor);
	
}

void AJournalManager::BeginPlay()
{
	Super::BeginPlay();
	CaptureMap();
	AvailableCollectibles.Empty();
	AvailableCheckpoints.Empty();
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	SaveGame = SaveSubsystem->LoadGame();
	;
	for (ACollectableItems* Collectable : CollectableItems)
	{
		if (!SaveGame->FindInteractedActor(Collectable->UniqueIDComponent->UniqueID))
		{
			AvailableCollectibles.Add(Collectable);
			Collectable -> OnObjectCollected.AddDynamic(this, &AJournalManager::UpdateCollectable);
		}
		else
		{
			Collectable -> SetActorHiddenInGame(true);
			Collectable -> SetActorEnableCollision(false);
			Collectable -> SetActorTickEnabled(false);
		}
	}
	for (ACheckpoint* Checkpoint : Checkpoints)
	{
		if (SaveGame->UnlockedTeleportPoints.Contains(Checkpoint->GetUniqueIDComponent_Implementation()->UniqueID))
		{
			AvailableCheckpoints.AddUnique(Checkpoint);
		}
		else
			Checkpoint -> OnCheckpointInteracted.AddDynamic(this, &AJournalManager::UpdateAvailableCheckpoints);
	}
	
}

