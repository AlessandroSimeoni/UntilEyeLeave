#include "Creatures/Projectiles/EyeProjectile.h"
#include "Eyes/EyesHandlerActorComponent.h"

AEyeProjectile::AEyeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEyeProjectile::BeginPlay()
{
	Super::BeginPlay();

	EyeSceneComponent = FindComponentByClass<UEyeSceneComponent>();
	
}

void AEyeProjectile::Shoot(const UMuzzle* InstigatorMuzzle, const float ProjectileSpeed, const AActor* ProjectileTargetActor)
{
	Super::Shoot(InstigatorMuzzle, ProjectileSpeed, ProjectileTargetActor);
	if (EyeSceneComponent)
	{
		EyeSceneComponent->OnDeath.AddDynamic(this, &AEyeProjectile::ProjectileDestroyed);

		// assign player if target is not assigned
		if (!Target.IsValid())
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (IsValid(PlayerPawn))
			{
				Target = PlayerPawn;
			}
		}

		// set eye rotation
		if (Target.IsValid())
		{
			EyeSceneComponent->SetEyeTarget(Target.Get());
		}
	}
}

void AEyeProjectile::ProjectileDestroyed(UEyeSceneComponent* This, const AActor* Destroyer)
{
	OnEyeProjectileDestroyed.Broadcast();
	DestructionFX.SpawnEffect(GetWorld(),GetActorLocation());
	Destroy();
}

