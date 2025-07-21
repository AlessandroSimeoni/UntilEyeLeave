// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/SpikeProjectile.h"

void ASpikeProjectile::BroadcastLoadOver()
{
	OnLoadOver.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Tick attivo su %s"), *GetName());
	SetActorTickEnabled(false);
}

void ASpikeProjectile::BroadcastReadyToShoot()
{
	OnReadyToShoot.Broadcast();
}

void ASpikeProjectile::HandleCollision()
{
	Super::HandleCollision();

	//AttachToActor(TraceHit.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
}

void ASpikeProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void ASpikeProjectile::Shoot(const UMuzzle* InstigatorMuzzle, const float ProjectileSpeed,
                             const AActor* ProjectileTargetActor)
{
	SetActorTickEnabled(true);
	Super::Shoot(InstigatorMuzzle, ProjectileSpeed, ProjectileTargetActor);
	float RandomYaw = FMath::RandRange(-RandomAngleShoot, RandomAngleShoot);
	UE_LOG(LogTemp, Warning, TEXT("random angle %f"), RandomAngleShoot);
	FRotator CurrentRotation = GetActorRotation();
	
	CurrentRotation.Yaw += RandomYaw;
	
	SetActorRotation(CurrentRotation);
}
