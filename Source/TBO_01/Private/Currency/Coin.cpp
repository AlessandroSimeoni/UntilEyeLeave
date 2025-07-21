#include "Currency/Coin.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"
#include "SaveSystem/UniqueIDComponent.h"
#include "Character/MainCharacter.h"
#include "Creatures/B06/B06_Stealer.h"

ACoin::ACoin()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CoinCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CoinCollision"));
	CoinCollision->SetupAttachment(RootComponent);
	CoinCollision->SetBoxExtent(FVector(15.f, 15.f, 5.f));
	CoinCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CoinCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CoinCollision->SetEnableGravity(false);
	CoinCollision->SetSimulatePhysics(false);

	CoinCollision->BodyInstance.bLockXRotation = true;
	CoinCollision->BodyInstance.bLockYRotation = true;
	CoinCollision->BodyInstance.bLockZRotation = true;

	RootComponent = CoinCollision;
	/*
	DestroyArea = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyArea"));
	DestroyArea->SetupAttachment(CoinCollision);
	DestroyArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroyArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	DestroyArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DestroyArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnDestroyAreaBeginOverlap);
*/
	MagneticArea = CreateDefaultSubobject<USphereComponent>(TEXT("MagneticArea"));
	MagneticArea->SetupAttachment(CoinCollision);
	MagneticArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MagneticArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	MagneticArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MagneticArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMagneticAreaBeginOverlap);

	UniqueIDComp = CreateDefaultSubobject<UUniqueIDComponent>(TEXT("UniqueID"));
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();
	if (SpawnLocation == FVector::ZeroVector)
	{
		SpawnLocation = GetActorLocation();
	}
	SetActorTickEnabled(false);
	SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (SaveSubsystem)
	{
		if (UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame())
		{
			if (TBOSaveGame->FindCoinActor(UniqueIDComp->UniqueID))
			{
				Destroy();
				UE_LOG(LogTemp,Warning,TEXT("Save Destroy"));
			}
		}
	}
}

void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsBeingPulled)
	{
		return;
	}

	if (!PullTargetActor.IsValid())
	{

		bIsBeingPulled = false;
		SetActorTickEnabled(false);
		EnableOverlapAreas(); 
		SetActorEnableCollision(true);
		CoinCollision->SetEnableGravity(false);
		CoinCollision->SetSimulatePhysics(false);
		return;
	}

	
	
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = PullTargetActor->GetActorLocation();
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * PullSpeed * DeltaTime;
	
	if (FVector::DistSquared(NewLocation, TargetLocation) < 100.f) 
	{
		SetActorLocation(TargetLocation);
		if (SaveSubsystem && !PullTargetActor->IsA(AB06_Stealer::StaticClass()))
		{
			if (UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame())
			{
				TBOSaveGame->AddCoin(UniqueIDComp->UniqueID);
			}
		}
		if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(PullTargetActor))
		{
			if (UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
			{
				FGameplayTag PickupTag = FGameplayTag::RequestGameplayTag(FName("Currency.PickUpCoin"));
				ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(PickupTag));
			}
		}
		bIsBeingPulled = false;
		Destroy();
		SetActorTickEnabled(false);
		return;
	}

	SetActorLocation(NewLocation);
}

void ACoin::MagneticPickUp(AActor* TargetActor)
{

	if (!TargetActor || (!TargetActor->IsA(AMainCharacter::StaticClass()) && !TargetActor->IsA(AB06_Stealer::StaticClass())))
	{
		return;
	}

	if (bIsBeingPulled && PullTargetActor.IsValid())
	{
		return;
	}

	MagneticArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	CoinCollision->SetSimulatePhysics(false);
	CoinCollision->SetEnableGravity(false);
	SetActorEnableCollision(false);

	bIsBeingPulled = true;
	PullTargetActor = TargetActor;
	SetActorTickEnabled(true);
}

void ACoin::SetGuid(FGuid Guid)
{
	if (UniqueIDComp)
	{
		UniqueIDComp->UniqueID = Guid;
	}
}

void ACoin::ApplyOppositeImpulse(const FVector& InputDirection, float Strength)
{
	if (!CoinCollision)
	{
		return;
	}

	
	MagneticArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	FVector Impulse =( FVector::UpVector * UpSpawnForceMultiplier + (InputDirection.GetSafeNormal())) * Strength;
	CoinCollision->SetSimulatePhysics(true);
	CoinCollision->SetEnableGravity(true);
	CoinCollision->AddImpulse(Impulse, NAME_None, true);

	if (bDrawDebugArrow)
	{
		const FVector StartLocation = GetActorLocation();
		const FVector EndLocation = StartLocation + Impulse * 0.05f; 
		DrawDebugDirectionalArrow(
			GetWorld(),
			StartLocation,
			EndLocation,
			50.f,            
			FColor::Red,
			false,           
			2.0f,            
			0,
			2.5f             
		);
	}
	
	GetWorldTimerManager().SetTimer(
		OverlapEnableTimerHandle,
		this,
		&ACoin::EnableOverlapAreas,
		OverlapActivationDelay,
		false 
	);
}

void ACoin::EnableOverlapAreas()
{
	MagneticArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MagneticArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}



void ACoin::OnMagneticAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MagneticPickUp(OtherActor);
}

void ACoin::ResetLocationToSpawn()
{
	if (!CoinCollision || !MagneticArea)
	{
		return;
	}


	MagneticArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	CoinCollision->SetSimulatePhysics(false);
	CoinCollision->SetEnableGravity(false);


	SetActorLocation(SpawnLocation, false, nullptr, ETeleportType::TeleportPhysics);

	
	CoinCollision->SetSimulatePhysics(true);
	CoinCollision->SetEnableGravity(true);

	const FVector UpImpulse = FVector::UpVector * 100.f; 
	CoinCollision->AddImpulse(UpImpulse, NAME_None, true);


	GetWorldTimerManager().SetTimer(
		OverlapEnableTimerHandle,
		this,
		&ACoin::EnableOverlapAreas,
		OverlapActivationDelay,
		false
	);
}