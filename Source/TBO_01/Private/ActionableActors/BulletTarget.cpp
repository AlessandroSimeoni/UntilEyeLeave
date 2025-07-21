// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/BulletTarget.h"
#include "Creatures/Projectiles/BaseProjectile.h"

#include "NiagaraComponent.h"

ABulletTarget::ABulletTarget()
{
	TargetMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMeshComponent"));
	TargetMeshComponent->SetupAttachment(RootComponent);

	HintVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HintVFX"));
	HintVFX->SetupAttachment(RootComponent);
}

void ABulletTarget::ChangeMaterial() const
{
	TargetMeshComponent->SetMaterial(0, ActiveTargetMaterial);
}

bool ABulletTarget::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator,float Intensity)
{
	if (Activated)
		return false;
	
	for (TSubclassOf<ABaseProjectile> ProjectileClass : ProjectileClasses)
		if (AttackInstigator->IsA(ProjectileClass))
		{
			ActionActors();
			OnTargetHit();
			return true;
		}
	
	return false;
}

void ABulletTarget::RestoreState_Implementation()
{
	ChangeMaterial();
}

void ABulletTarget::ActionActors(const bool InstantActivation)
{
	Super::ActionActors(InstantActivation);

	ChangeMaterial();
}
