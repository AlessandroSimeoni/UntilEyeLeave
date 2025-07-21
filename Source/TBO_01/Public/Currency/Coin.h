#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

class UBoxComponent;
class USphereComponent;
class UUniqueIDComponent;
class AMainCharacter;
class USaveSubsystem;

UCLASS()
class TBO_01_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:
	
	ACoin();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Coin|Physics")
	void ApplyOppositeImpulse(const FVector& InputDirection, float Strength = 800.f);
	void EnableOverlapAreas();

	UFUNCTION(BlueprintCallable, Category = "Currency")
	void SetGuid(FGuid Guid);

	void ResetLocationToSpawn();
	
	FVector SpawnLocation = FVector::ZeroVector;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coin")
	int32 CoinValue = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	float OverlapActivationDelay = 1.0f; 

	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	float UpSpawnForceMultiplier = 1.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coin")
	bool bDrawDebugArrow = false;
	
	FTimerHandle OverlapEnableTimerHandle;
	
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trigger Areas")
	TObjectPtr<USphereComponent> MagneticArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> CoinCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorComponent")
	TObjectPtr<UUniqueIDComponent> UniqueIDComp = nullptr;

	TObjectPtr<USaveSubsystem> SaveSubsystem = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Magnetism")
	float PullAwayDistance = 5.0f;

	FVector PullTargetLocation;
	TWeakObjectPtr<AActor> PullTargetActor;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Movement|Magnetism")
	float PullSpeed = 600.f; 

	
	bool PlayerInMagneticRange = false;

	// Movement state
	bool bIsBeingPulled = false;

	
	
	


	UFUNCTION()
	void OnMagneticAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void MagneticPickUp(AActor* TargetActo);
	
};

