// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Action/ShootingAbility.h"

#include "Creatures/Projectiles/Muzzle.h"
#include "Creatures/Projectiles/MuzzlesController.h"
#include "Interface/Shooter.h"

void UShootingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthority(&ActivationInfo)) return;
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	if (TriggerEventData->Instigator && TriggerEventData->Instigator->Implements<UShooter>())
	{
		TScriptInterface<IShooter> ShooterInstance = const_cast<AActor*>(TriggerEventData->Instigator.Get());
		UMuzzlesController* MuzzlesController = ShooterInstance->Execute_GetMuzzlesController(const_cast<AActor*>(TriggerEventData->Instigator.Get()));

		if (MuzzlesController)
		{
			TArray<UMuzzle*> Muzzles = MuzzlesController->GetAvailableMuzzles();
			for (UMuzzle* Muzzle : Muzzles)
				Muzzle->Shoot(ProjectileClass, ProjectileSpeed, TriggerEventData->Target.Get());

			if (Muzzles.Num() > 0)
				UE_LOG(LogTemp,Warning,TEXT("ShootingAbility:: %s"),*TriggerEventData->Instigator->GetName());
		}
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
