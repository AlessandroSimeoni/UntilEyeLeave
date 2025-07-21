// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/StreetLamp.h"

#include "Character/MainCharacter.h"
#include "Creatures/Creature.h"
#include "Creatures/B04/B04_CageBird.h"
#include "Creatures/B04/B04_Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"
#include "SaveSystem/UniqueIDComponent.h"

AStreetLamp::AStreetLamp()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	UniqueIDComp = CreateDefaultSubobject<UUniqueIDComponent>(TEXT("UniqueID"));
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	
	BirdSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BirdSpawnPoint"));
	BirdSpawnPoint->SetupAttachment(RootComponent);

	BirdNestPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BirdNestPoint"));
	BirdNestPoint->SetupAttachment(RootComponent);
}

bool AStreetLamp::HasBeenActivated_Implementation()
{
	return Activated;
}

bool AStreetLamp::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity)
{
	if (Activated)
		return false;

	if (AttackInstigator && AttackInstigator->IsA(AMainCharacter::StaticClass()))
		return false;
	
	if (ResistanceLevel > CharacterLevel)
		return false;

	BirdFalling();
	OnStreetLampHit();
	Activated = true;
	
	return true;
}

void AStreetLamp::BirdFalling()
{
	if (!Bird)
		return;
	
	FGameplayTagContainer AbilityTags;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Action.Fall")));
	Bird->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AbilityTags);
}

void AStreetLamp::HandleAltarReached()
{
	UTBOSaveGame* TBOSaveGame = GetGameInstance()->GetSubsystem<USaveSubsystem>()->LoadGame();
	TBOSaveGame->AddInteractedActor(UniqueIDComp->UniqueID);
}


void AStreetLamp::BeginPlay()
{
	Super::BeginPlay();

	UTBOSaveGame* TBOSaveGame = GetGameInstance()->GetSubsystem<USaveSubsystem>()->LoadGame();
	if (!TBOSaveGame->FindInteractedActor(UniqueIDComp->UniqueID))
	{
		//spawn bird
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Bird = GetWorld()->SpawnActor<AB04_CageBird>(BirdClass, BirdSpawnPoint->GetComponentLocation(), BirdSpawnPoint->GetComponentRotation(), SpawnParams);
		AB04_Controller* BirdController = GetWorld()->SpawnActor<AB04_Controller>(BirdControllerClass, BirdSpawnPoint->GetComponentLocation(), BirdSpawnPoint->GetComponentRotation(), SpawnParams);
		BirdController->Possess(Bird);
		BirdController->AssignNestPosition(BirdNestPoint->GetComponentLocation());
		BirdController->OnReachedAltar.AddDynamic(this, &ThisClass::HandleAltarReached);
		Bird->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		
		if (HasFallenLantern)
		{
			Bird->SetFree(true);
			Bird->Teleport(BirdNestPoint->GetComponentLocation(), MOVE_Walking);
			Activated = true;
		}

		if (LanternStartsActive)
			Bird->SelfCharge();
	}
	else
		Activated = true;
}
