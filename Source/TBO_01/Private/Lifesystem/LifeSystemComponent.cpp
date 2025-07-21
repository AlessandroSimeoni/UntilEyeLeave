#include "Lifesystem/LifeSystemComponent.h"

void ULifeSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void ULifeSystemComponent::ReceiveHeal(float Value)
{
	if (!Immortal)
	{
		// avoid healing when already full
		if (CurrentHealth == MaxHealth)
		{
			return;
		}
		
		CurrentHealth = FMath::Min(CurrentHealth + Value, MaxHealth);
		OnHealReceived.Broadcast(this, Value);
	}
}

void ULifeSystemComponent::ReceiveDamage(float Value)
{
	if (!Immortal)
	{
		// avoid receiving damage when already dead
		if (CurrentHealth == 0.0f)
		{
			return;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Damage Received %f"), Value);

		CurrentHealth -= Value;
		OnDamageTaken.Broadcast(this, Value);
		
		if(CurrentHealth < 0.0f)
			CurrentHealth = 0.0f;
		
		UE_LOG(LogTemp, Warning, TEXT("Current Health = %f"), GetCurrentHealth());
		
		if (FMath::IsNearlyEqual(CurrentHealth, 0.0f))
			OnDeath.Broadcast(this);
	}
}

void ULifeSystemComponent::SetCurrentHealth(float Value)
{
	CurrentHealth = Value;
}
