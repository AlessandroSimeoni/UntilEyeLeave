// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/B05Boss/B05_BossHead_Controller.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MainCharacter.h"
#include "Creatures/Creature.h"
#include "Creatures/B05Boss/B05_BossHand.h"
#include "Creatures/B05Boss/B05_BossHead.h"
#include "Creatures/Projectiles/EyeProjectile.h"
#include "Lifesystem/LifeSystemComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_MinionB01, "Action.SpawnMinion.B01")
UE_DEFINE_GAMEPLAY_TAG(TAG_MinionB02, "Action.SpawnMinion.B02")
UE_DEFINE_GAMEPLAY_TAG(TAG_GetDamage, "FeedBack.HitBig")
UE_DEFINE_GAMEPLAY_TAG(TAG_RespawnEye, "Action.Boss.RespawnEye")
// Sets default values
AB05_BossHead_Controller::AB05_BossHead_Controller()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AB05_BossHead_Controller::BeginPlay()
{
	Super::BeginPlay();
}

void AB05_BossHead_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BossHead = Cast<AB05_BossHead>(InPawn);
	ASC = BossHead->GetAbilitySystemComponent();
}


// Called every frame
void AB05_BossHead_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AB05_BossHead_Controller::ResetBoss()
{
	CurrentPhase = static_cast<EBossPhase>(0);
	if (CurrentMinion.IsValid())
	{
		CurrentMinion->Destroy();
	}


	if (Blackboard)
	{
		Blackboard->SetValueAsInt(KeyNamePhase, 0);
	}

	BossHead->SetEyesActive();
	BossHead->DestroyHands();

	ClearAllLooseTags();
}



void AB05_BossHead_Controller::NextPhase()
{
	int32 PhaseInt = static_cast<int32>(CurrentPhase);
	PhaseInt++;
	CurrentPhase = static_cast<EBossPhase>(PhaseInt);
	OnPhaseChange.Broadcast(CurrentPhase);
	CheckPhase();

	if (Blackboard)
	{
		Blackboard->SetValueAsInt(KeyNamePhase, PhaseInt);
	}
}

void AB05_BossHead_Controller::CheckPhase()
{
	switch (CurrentPhase)
	{
	case EBossPhase::Phase2:
		{
			//set the restPoint for the hand and that influence the behavior tree
			BossHead->SetHandsToRest();
			FGameplayTag TAG_Activate = FGameplayTag::RequestGameplayTag(FName("FeedBack.Activation"));
			BossHead->DestructibleHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_Activate));
		//	BossHead->DestructibleHand->SetActiveStatus(true);
			//spawnB02 Minion and in the ability Set Current Minion
			BossHead->DestructibleHand->OnDeath.AddDynamic(this, &AB05_BossHead_Controller::GetDamage);
			ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_MinionB02));
			AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			Player->GetLifeComponent()->OnDeath.AddDynamic(this, &AB05_BossHead_Controller::CallResetOnPlayerDeath);
			break;
		}

	case EBossPhase::Phase3:
		{
			FGameplayTag TAG_InstantDeathB02 = FGameplayTag::RequestGameplayTag(FName("FeedBack.Death.InstantDeath"));
			//destroy Minion avoid event activation in phase3
			CurrentMinion->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
				FGameplayTagContainer(TAG_InstantDeathB02));
			break;
		}

	case EBossPhase::Phase4:
		{
			FGameplayTag TAG_Activate = FGameplayTag::RequestGameplayTag(FName("FeedBack.Activation"));
			BossHead->StepOnHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_Activate));
			//BossHead->StepOnHand->SetActiveStatus(true);
			ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_MinionB01));
			break;
		}

	case EBossPhase::Death:
		{
			FGameplayTag TAG_SlowDeath = FGameplayTag::RequestGameplayTag(FName("FeedBack.Death.SlowDeath"));
			FGameplayTag TAG_InstantDeathB01 = FGameplayTag::RequestGameplayTag(FName("FeedBack.Death.InstantDeath"));
			CurrentMinion->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_InstantDeathB01));
			UE_LOG(LogTemp,Warning,TEXT("current %s"), *BossHead->StepOnHand->GetName());
			BossHead->StepOnHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_SlowDeath));
			//BossHead->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_SlowDeath));

			break;
		}
	default:
		{
			break;
		}
	
	}
}

void AB05_BossHead_Controller::SetCurrentMinion(ACreature* Minion)
{

	CurrentMinion = Minion;
	Minion->OnDeath.AddDynamic(this, &AB05_BossHead_Controller::CheckMinionDeath);
}

void AB05_BossHead_Controller::CheckMinionDeath(AActor* Actor, const AActor* InstigatorActor)
{
	UE_LOG(LogTemp,Warning,TEXT("start CheckMinionDeath"));

	switch (CurrentPhase)
	{
	case EBossPhase::Phase2:
		{
			ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_MinionB02));
			break;
		}
	case EBossPhase::Phase4:
		{
			ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_MinionB01));
			break;
		}
	default:
		{
			break;
		}
	
	}
}

void AB05_BossHead_Controller::CheckEyeProjectile(AEyeProjectile* EyeProjectile)
{
	EyeProjectile->OnEyeProjectileDestroyed.AddDynamic(this, &AB05_BossHead_Controller::GetDamage);
	EyeProjectile->OnProjectileEndLifeCycle.AddDynamic(this, &AB05_BossHead_Controller::RespawnMissingEye);
}

void AB05_BossHead_Controller::RespawnMissingEye()
{
	BossHead->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_RespawnEye));
}

void AB05_BossHead_Controller::GetDamage()
{
	BossHead->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GetDamage));
}

void AB05_BossHead_Controller::GetDamage(AActor* Actor, const AActor* DeathInstigator)
{
	BossHead->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GetDamage));
}

void AB05_BossHead_Controller::ClearAllLooseTags()
{
	if (!ASC)
	{
		return;
	}

	FGameplayTagContainer AllTags;
	ASC->GetOwnedGameplayTags(AllTags);

	for (const FGameplayTag& Tag : AllTags)
	{
		if (ASC->HasMatchingGameplayTag(Tag))
		{
			ASC->RemoveLooseGameplayTag(Tag);
		}
	}
}

void AB05_BossHead_Controller::CallResetOnPlayerDeath(USceneComponent* PlayerLifeComponent)
{
	Cast<ULifeSystemComponent>(PlayerLifeComponent)->OnDeath.RemoveDynamic(this, &AB05_BossHead_Controller::CallResetOnPlayerDeath);
	ResetBoss();
}
