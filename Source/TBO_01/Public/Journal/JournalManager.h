#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JournalManager.generated.h"

enum class EMapIconType : uint8;
class ACheckpoint;
class ATargetPoint;
class ACollectableItems;
struct FJournalEntry;
class UTBOSaveGame;
enum class EPageType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFragmentCollected, FName, ENtryID, EPageType, Section, bool, IsTrigger);

USTRUCT(BlueprintType)
struct FMapBounds
{
	GENERATED_BODY()

public:
	FMapBounds()
	:Min(FVector2D::ZeroVector),Max(FVector2D::ZeroVector){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map Bounds")
	FVector2D Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map Bounds")
	FVector2D Max;

	/** Returns the size of the bounds. */
	FORCEINLINE FVector2D GetSize() const
	{
		return Max - Min;
	}
};

UCLASS()
class TBO_01_API AJournalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AJournalManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	UDataTable* BestiaryDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	UDataTable* IconData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TArray<ACollectableItems*> CollectableItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TArray<ACheckpoint*> Checkpoints;
	
	UPROPERTY(BlueprintAssignable, Category = "Journal")
	FOnFragmentCollected OnFragmentCollected;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	int IndexToOpen = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapRender")
	ATargetPoint* UpperLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapRender")
	ATargetPoint* LowerRight;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapRender")
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapRender")
	UTextureRenderTarget2D* RenderTarget;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map Render")
	FMapBounds MapBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TArray<ACollectableItems*> AvailableCollectibles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TArray<ACheckpoint*> AvailableCheckpoints;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTBOSaveGame* SaveGame;
	
protected:
	virtual void BeginPlay() override;

private:
	
	UFUNCTION()
	void UpdateCollectable(ACollectableItems* Actor);

public:

	UFUNCTION(BlueprintCallable, Category = "Journal")
	void CollectFragment(FName EntryID, EPageType Section, TArray<int32> Indexes, bool IsTrigger);

	UFUNCTION(CallInEditor, Category = "Map")
	void GetAllCollectibles();

	UFUNCTION(CallInEditor, Category = "Map")
	void GetAllCheckpoints();
	
	UFUNCTION(CallInEditor, Category = "MapRender")
	void CaptureMap();

	UFUNCTION(BlueprintCallable, Category="Map Render")
	FMapBounds GetMapBounds() const;
	
	UFUNCTION(BlueprintPure, Category = "Map")
	FVector2D ConvertWorldToMapLocation(const FVector& WorldLocation, const FMapBounds& Bounds, const FVector2D& MapImageSize);

	UFUNCTION(BlueprintPure, Category = "Map")
	UTexture2D* GetIconFromDataTable(EMapIconType IconType, bool Collected);

	UFUNCTION()
	void UpdateAvailableCheckpoints(ACheckpoint* Checkpoint);
};
