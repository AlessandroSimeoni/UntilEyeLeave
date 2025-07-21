// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossAiControllor.h"
#include "NativeGameplayTags.h"
#include "B05_BossHead_Controller.generated.h"

class AEyeProjectile;
class AB05_BossHead;
class ACreature;

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Sleep UMETA(DisplayName = "Sleeping"),
	Intro UMETA(DisplayName = "Intro"),
	Phase1 UMETA(DisplayName = "Pahse1"),
	Phase2 UMETA(DisplayName = "Pahse2"),
	Phase3 UMETA(DisplayName = "Pahse3"),
	Phase4 UMETA(DisplayName = "Pahse4"),
	Death UMETA(DisplayName = "Death")
};

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MinionB01);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MinionB02);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_RespawnEye);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GetDamage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Transition);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FonPhaseChange,EBossPhase, BossPhase);

UCLASS()
class TBO_01_API AB05_BossHead_Controller : public ABossAiControllor
{
	GENERATED_BODY()

public:
	AB05_BossHead_Controller();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable,Category="Boss")
	FonPhaseChange OnPhaseChange;
	
	//**Set the phase to sleep kill all minion kill hands clear gameplay tag, reset little eye visibility*/
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void ResetBoss();


protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	
	UFUNCTION(BlueprintCallable, Category = "BossPhase")
	void NextPhase();

	/** Set The event to check if the minion Die*/
	UFUNCTION(BlueprintCallable, Category = "Minion")
	void SetCurrentMinion(ACreature* Minion);

	/**Respawn minion based on phase*/
	UFUNCTION()
	void CheckMinionDeath(AActor* Actor, const AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Eyes")
	void CheckEyeProjectile(AEyeProjectile* EyeProjectile);

	UFUNCTION(BlueprintCallable, Category = "Eyes")
	void RespawnMissingEye();

	UFUNCTION(BlueprintCallable, Category = "Eyes")
	void GetDamage();
	
	void GetDamage(AActor* Actor, const AActor* DeathInstigator);
	
	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNamePhase = "Phase";

	
private:
	void CheckPhase();
	void ClearAllLooseTags();

	UPROPERTY(BlueprintReadOnly,Category="Boss",meta=(AllowPrivateAccess=true))
	TObjectPtr<AB05_BossHead> BossHead;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	TWeakObjectPtr<ACreature> CurrentMinion;

	UPROPERTY(BlueprintReadWrite,Category="Boss",meta=(AllowPrivateAccess=true))
	EBossPhase CurrentPhase = EBossPhase::Sleep;

	UFUNCTION()
	 void CallResetOnPlayerDeath(USceneComponent* PlayerLifeComponent);
};
