// Fill out your copyright notice in the Description page of Project Settings.


#include "Lifesystem/HealingItem.h"

#include "Character/MainCharacter.h"
#include "Components/SphereComponent.h"
#include "Lifesystem/LifeSystemComponent.h"

AHealingItem::AHealingItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	DestroyArea = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyArea"));
	DestroyArea->SetupAttachment(RootComponent);
	DestroyArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroyArea->SetCollisionObjectType(ECC_WorldDynamic);
	DestroyArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	DestroyArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DestroyArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnDestroyAreaBeginOverlap);

	MagneticArea = CreateDefaultSubobject<USphereComponent>(TEXT("MagneticArea"));
	MagneticArea->SetupAttachment(RootComponent);
	MagneticArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MagneticArea->SetCollisionObjectType(ECC_WorldDynamic);
	MagneticArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	MagneticArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MagneticArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMagneticAreaBeginOverlap);
	MagneticArea->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnMagneticAreaEndOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHealingItem::OnDestroyAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!PlayerInMagneticRange)
	{
		Character = Cast<AMainCharacter>(OtherActor);
		if (!Character)
			return;

		if (Character->GetLifeComponent()->GetCurrentHealth() == Character->GetLifeComponent()->MaxHealth)
			return;
	}
	
	Character->GetLifeComponent()->ReceiveHeal(HealingValue);
	Destroy();
}

void AHealingItem::OnMagneticAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	Character = Cast<AMainCharacter>(OtherActor);
	if (!Character)
		return;

	if (Character->GetLifeComponent()->GetCurrentHealth() == Character->GetLifeComponent()->MaxHealth)
	{
		Character->GetLifeComponent()->OnDamageTaken.AddDynamic(this, &ThisClass::ActivatePickUp);
		return;
	}

	ActivatePickUp();
}

void AHealingItem::OnMagneticAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Character)
	{
		return;
	}
	
	Character->GetLifeComponent()->OnDamageTaken.RemoveDynamic(this, &ThisClass::ActivatePickUp);
}

void AHealingItem::ActivatePickUp(USceneComponent* LifeComponent, float DamageReceived)
{
	if (!Character)
	{
		return;
	}
	
	Character->GetLifeComponent()->OnDamageTaken.RemoveDynamic(this, &ThisClass::ActivatePickUp);
	
	PlayerInMagneticRange = true;
	MagneticArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const FVector CharacterToTargetDirection = (GetActorLocation() - Character->GetActorLocation()).GetUnsafeNormal();
	FRotator TargetRotation = CharacterToTargetDirection.Rotation();
	TargetRotation.Yaw += FMath::RandRange(MinPullAwayHorizontalDegrees, MaxPullAwayHorizontalDegrees);
	TargetRotation.Pitch += FMath::RandRange(MinPullAwayVerticalDegrees, MaxPullAwayVerticalDegrees);
	MagneticPickUp(GetActorLocation() + TargetRotation.Vector()*PullAwayDistance);
}

void AHealingItem::BeginPlay()
{
	Super::BeginPlay();

	Period = 1 / FloatingFrequency;
	InitialZLocation = Mesh->GetComponentLocation().Z;
}

void AHealingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// rotation
	FRotator NewRotation = Mesh->GetComponentRotation();
	NewRotation.Yaw += RotationSpeed * DeltaTime;
	Mesh->SetWorldRotation(NewRotation);

	// floating
	FVector NewLocation = Mesh->GetComponentLocation();
	CurrentTime = FMath::Fmod(CurrentTime + DeltaTime, Period); 
	NewLocation.Z = InitialZLocation + FloatingDistance * FMath::Sin(2 * PI * FloatingFrequency * CurrentTime);
	Mesh->SetWorldLocation(NewLocation);
}
