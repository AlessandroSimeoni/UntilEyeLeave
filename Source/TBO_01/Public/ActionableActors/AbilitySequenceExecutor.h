#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Creatures/DataAsset/CreatureAbilitySequence.h" 
#include "AbilitySequenceExecutor.generated.h"


USTRUCT()
struct FAbilitySequenceState
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCreatureAbilitySequence> Sequence;

	UPROPERTY()
	int32 Index = 0;

	FTimerHandle TimerHandle;
};

USTRUCT(BlueprintType)
struct FRegisteredAbilitySequence
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UCreatureAbilitySequence> Sequence;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TWeakObjectPtr<AActor> Target;
};

UCLASS()
class TBO_01_API AAbilitySequenceExecutor : public AActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void RegisterAbilitySequenceWithTarget(UCreatureAbilitySequence* Sequence, AActor* OwnerActor, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void StartAbilitySequence(UCreatureAbilitySequence* Sequence, AActor* OwnerActor);

	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void StopAbilitySequence(AActor* OwnerActor);

	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void StartAllAbilitySequences();

	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void StopAllAbilitySequences();

	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void TriggerNextEventAbility(AActor* OwnerActor);

protected:

	UPROPERTY(EditAnywhere)
	TMap<TWeakObjectPtr<AActor>, FRegisteredAbilitySequence> RegisteredSequences;

	UPROPERTY()
	TMap<TWeakObjectPtr<AActor>, FAbilitySequenceState> ActiveSequences;

	void ScheduleNextAbility(AActor* OwnerActor);

	UFUNCTION()
	void ExecuteAbility(AActor* OwnerActor);
private:
	void ActivateAbility(AActor* OwnerActor, const FAbilityDelay& AbilityData);
};
