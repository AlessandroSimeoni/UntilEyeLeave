// Fill out your copyright notice in the Description page of Project Settings.


#include "Currency/CoinContainer.h"

#include "NiagaraFunctionLibrary.h"
#include "Currency/Coin.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"


// Sets default values for this component's properties
UCoinContainer::UCoinContainer()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCoinContainer::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (!UniqueID.IsValid())
	{
		UniqueID = FGuid::NewGuid();
	}
}


void UCoinContainer::BeginPlay()
{
	Super::BeginPlay();
	
	USaveSubsystem* SaveSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame();
	if (TBOSaveGame->FindCoinActor(UniqueID))
	{
		CoinNumber = 0;
	}
	CheckCoin();
}

void UCoinContainer::CheckCoin()
{
	if (CoinNumber > 0 && CoinNiagaraFX)
	{
		SpawnedNiagaraFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
			CoinNiagaraFX,
			this,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true 
		);
	}
}

void UCoinContainer::SpawnCoin(const FVector& InstigatorPosition, FVector SpawnLoaction)
{
	if (CoinNumber <= 0 || !GetWorld())
	{
		return;
	}

	if (CoinSpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CoinSpawnSound, GetComponentLocation());
	}
	
	const FVector SpawnOrigin = GetComponentLocation();
	
	FVector BaseDirection = UKismetMathLibrary::RandomUnitVector();
	if (!InstigatorPosition.IsNearlyZero())
	{
		FVector FlatDir = SpawnOrigin - InstigatorPosition;
		FlatDir.Z = 0.f;
		BaseDirection = FlatDir.GetSafeNormal();
	}

	for (int32 i = 0; i < CoinNumber; ++i)
	{
		const FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, CoinSpawnInstanceRandomOffsetSphereRadius);
		const FVector SpawnLocation = SpawnOrigin + RandomOffset;
		//FVector DirectionVariance = UKismetMathLibrary::RandomUnitVector() * 0.3f;
		//DirectionVariance.Z = 0.f;

		FVector FinalDirection = BaseDirection;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (ACoin* SpawnedCoin = GetWorld()->SpawnActor<ACoin>(CoinClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams))
		{
			if (!SpawnLoaction.IsNearlyZero())
			{
				SpawnedCoin ->SpawnLocation = SpawnLoaction;
			}
			SpawnedCoin->SetGuid(UniqueID);
			SpawnedCoin->ApplyOppositeImpulse(FinalDirection, CoinsSpawnStrenght);
		}
	}

	CoinNumber = 0;
}

void UCoinContainer::SetCoin(int32 value)
{
	CoinNumber = value;
	CheckCoin();
}
