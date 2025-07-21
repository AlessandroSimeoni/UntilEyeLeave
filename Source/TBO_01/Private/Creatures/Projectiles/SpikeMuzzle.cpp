// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/SpikeMuzzle.h"

#include "Creatures/Projectiles/SpikeProjectile.h"

void USpikeMuzzle::PrepareNextShot()
{
	FVector Offset = GetForwardVector() * SpawnOffsetFromMuzzle.X + GetRightVector() * SpawnOffsetFromMuzzle.Y + GetUpVector() * SpawnOffsetFromMuzzle.Z;
	SpikeProjectileInstance = GetWorld()->SpawnActor<ASpikeProjectile>(SpikeProjectileClass, GetComponentLocation() + Offset, GetComponentRotation());
	SpikeProjectileInstance->OnLoadOver.AddDynamic(this, &ThisClass::HandleSpikeReady);
	SpikeProjectileInstance->LoadProjectile(MuzzleCooldownTime,GetComponentLocation());
}

void USpikeMuzzle::ShootSpike()
{
	SpikeProjectileInstance->OnReadyToShoot.RemoveDynamic(this, &ThisClass::ShootSpike);
	SpikeProjectileInstance->Shoot(this, Speed, nullptr);
	ReadyToShoot = false;
	PrepareNextShot();
}

void USpikeMuzzle::HandleSpikeReady()
{
	SpikeProjectileInstance->OnLoadOver.RemoveDynamic(this, &ThisClass::HandleSpikeReady);
	ReadyToShoot = true;
}

void USpikeMuzzle::BeginPlay()
{
	Super::BeginPlay();

	SpikeProjectileInstance = GetWorld()->SpawnActor<ASpikeProjectile>(SpikeProjectileClass, GetComponentLocation(),
	                                                                   GetComponentRotation());
}

void USpikeMuzzle::OnComponentDestroyed(bool bPromoteChildren)
{
	Super::OnComponentDestroyed(bPromoteChildren);
	if (SpikeProjectileInstance)
	{
		SpikeProjectileInstance->Destroy();
	}
}

ABaseProjectile* USpikeMuzzle::Shoot(const TSubclassOf<ABaseProjectile> ProjectileClass, const float ProjectileSpeed,
                                     const AActor* ProjectileTargetActor, const float Damage, const float Destruction,  const float KnockbackIntensity,const float RandomAngle)
{
	if(!ReadyToShoot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Muzzle %s not ready to shoot"), *GetName());	
		return nullptr;
	}
	

	Speed = ProjectileSpeed;
	SpikeProjectileInstance->SetDamage(Damage);
	SpikeProjectileInstance->SetDestruction(Destruction);
	SpikeProjectileInstance->SetKnockbackIntensity(KnockbackIntensity);
	SpikeProjectileInstance->RandomAngleShoot = RandomAngle;
	
	
	SpikeProjectileInstance->OnReadyToShoot.AddDynamic(this, &ThisClass::ShootSpike);
	SpikeProjectileInstance->StartShoot();
	return SpikeProjectileInstance;
}
