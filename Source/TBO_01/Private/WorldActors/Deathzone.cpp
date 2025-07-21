// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/Deathzone.h"

#include "EngineUtils.h"
#include "Character/CharacterController.h"
#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerCombatState.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Creatures/Creature.h"
#include "Currency/Coin.h"
#include "FSM/StateBase.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Lifesystem/LifeSystemComponent.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"

// Sets default values
ADeathzone::ADeathzone()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DeathArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathArea"));
	DeathArea->SetupAttachment(RootComponent);
	DeathArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DeathArea->SetCollisionObjectType(ECC_WorldStatic);
	DeathArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	DeathArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DeathArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnDeathAreaBeginOverlap);
}

void ADeathzone::OnDeathAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACreature* Creature = Cast<ACreature>(OtherActor);
	if (Creature)
	{
		Creature->InstantDeath();
		return;
	}

	ACoin * Coin = Cast<ACoin>(OtherActor);

	if (Coin)
	{
		Coin->ResetLocationToSpawn();
	}
	
	Character = Cast<AMainCharacter>(OtherActor);
	if (!Character)
		return;

	Character->GetLifeComponent()->ReceiveDamage(DeathzoneDamage);
	Character->StopAnimMontage();

	// handle combat state otherwise it remains in that state locking the player
	if (Character->StateControllerComponent->CurrentState->IsA(UPlayerCombatState::StaticClass()))
		Character->StateControllerComponent->ForceDefaultState();
	
	FTimerManager& TM = GetWorld()->GetTimerManager();

	if (Character->GetLifeComponent()->GetCurrentHealth() > 0.0f)
	{
		// disable collision response to pawn
		DeathArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Character->MovementComponent->StopMovementImmediately();

		if (TeleportToCheckpoint)
		{
			// teleport player
			USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
			UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
			if (SaveGame->LastCheckpointLocation != FVector::ZeroVector)
				Character->SetActorLocation(SaveGame->LastCheckpointLocation + FVector::UpVector * Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			else
			{
				// teleport player to player start if last checkpoint is zero
				APlayerStart* PlayerStart = nullptr;
				for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
				{
					PlayerStart = *It;
					break;  // Take only the first player start
				}

				Character->SetActorLocation(PlayerStart->GetActorLocation() + FVector::UpVector * Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			}
		}
		else
		{
			Character->SetActorLocation(Character->GetLastGroundedLocation());

			// raycast to know if we are on the edge or not
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(Character);
			CollisionParams.AddIgnoredActor(this);
			const bool ThereIsGround = GetWorld()->LineTraceSingleByChannel(GroundCheckHit,
			                                                                Character->GetActorLocation(),
			                                                                Character->GetActorLocation() + FVector::DownVector * (Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + RayOffset),
			                                                                ECC_Visibility,
			                                                                CollisionParams);

			if (!ThereIsGround)
			{
				FVector CharacterDirection =  Character->GetActorLocation() - SweepResult.ImpactPoint;
				CharacterDirection.Z = 0.0f;
				CharacterDirection.Normalize();
				const FVector TeleportLocation = Character->GetActorLocation() + CharacterDirection * TeleportAwayOffset + FVector::UpVector * TeleportUpOffset;
				Character->SetActorLocation(TeleportLocation);
			}

			ACharacterController* Controller = Cast<ACharacterController>(Character->GetController());
			if (Controller)
			{
				Controller->ToggleControls(false);
				TM.SetTimer(InputInterruptionTH, this, &ThisClass::EnablePlayerControls, InputInterruptionCooldown, false);
			}
		}

		// set timer to reset collision response
		TM.SetTimer(TriggerAreaTH, this, &ThisClass::ResetCollisionResponse, 0.2f, false);
	}
}

void ADeathzone::ResetCollisionResponse() const
{
	DeathArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ADeathzone::EnablePlayerControls() const
{
	Cast<ACharacterController>(Character->GetController())->ToggleControls(true);
}

