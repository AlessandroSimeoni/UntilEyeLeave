#pragma once

#include "CoreMinimal.h"
#include "LifeBridgeInterface.h"
#include "Components/SceneComponent.h"
#include "LifeSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTaken, USceneComponent*, This, float, DamageReceived);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealReceived, USceneComponent*, This, float, HealthReceived);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, USceneComponent*, This);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TBO_01_API ULifeSystemComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable, Category="Life")
	FOnDamageTaken OnDamageTaken;
	UPROPERTY(BlueprintAssignable, Category="Life")
	FOnHealReceived OnHealReceived;
	UPROPERTY(BlueprintAssignable, Category="Life")
	FOnDeath OnDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Life", Meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Life")
	bool Immortal;
private:
	float CurrentHealth;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ReceiveHeal(float Value);
	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float Value);
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const { return CurrentHealth; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float Value);
	UFUNCTION(BlueprintCallable)
	void ResetCurrentHealth(){CurrentHealth = MaxHealth; }
};
