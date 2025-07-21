#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "CollectableItems.generated.h"

class UTBOSaveGame;
class USaveSubsystem;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectCollected, ACollectableItems*, Object);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionFailed);

UENUM(BlueprintType)
enum class EMapIconType : uint8
{
	MainCharacter   UMETA(DisplayName = "MainCharacter"),
	BestiaryPage    UMETA(DisplayName = "BestiaryPage"),
	LorePage		UMETA(DisplayName = "LorePage"),
	MapFragment     UMETA(DisplayName = "MapFragment"),
	HeartPiece      UMETA(DisplayName = "HeartPiece"),
	WeaponUpgrade   UMETA(DisplayName = "WeaponUpgrade"),
	Checkpoint      UMETA(DisplayName = "Checkpoint")
};

UCLASS()
class TBO_01_API ACollectableItems : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ACollectableItems();
	UPROPERTY()
	class UUniqueIDComponent* UniqueIDComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root")
	TObjectPtr<USceneComponent> MyRootComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collectables")
	class UBoxComponent* DetectionRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collectables")
	UBoxComponent* CollectionRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collectables")
	TObjectPtr<USceneComponent> InteractionPromptLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectables")
	UStaticMeshComponent* CollectableMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectables")
	UStaticMeshComponent* SecondaryCollectableMesh;
	
	UPROPERTY(BlueprintAssignable)
	FOnObjectCollected OnObjectCollected;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMapIconType MapIconType;
	
	UPROPERTY(BlueprintReadOnly)
	bool ItemCollected = false;
	UPROPERTY()
	bool ItemDiscovered = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTrigger = false;

	virtual void Interact_Implementation() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "On Interact Success"))
	void OnInteractSuccess();

	virtual USceneComponent* GetInteractionPromptSceneComponent_Implementation() override;

	//Coin
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price = 0;

	
	UPROPERTY(BlueprintAssignable)
	FOnInteractionFailed OnInteractionFailed;

protected:
	UPROPERTY()
	TObjectPtr<USaveSubsystem> SaveSubsystem = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTBOSaveGame> TBOSaveGame = nullptr;
	FTimerHandle TH;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCollectableSFX();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPurchaseSFX();

	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_SpendCoinsClass;
	
	virtual void BeginPlay() override;
	virtual void Discovered();
	virtual void Collected();
	virtual void HandleCollectedStatus();
	void CheckSaveFile();

	UFUNCTION()
	void OnDetectionRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
