// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/B04/B04_Controller.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MainCharacter.h"
#include "Creatures/B04/B04_CageBird.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LevelInstance/LevelInstanceTypes.h"
#include "Lifesystem/LifeSystemComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Charged, "Status.Charged")
UE_DEFINE_GAMEPLAY_TAG(TAG_FreeMove,"Status.FreeMove")
UE_DEFINE_GAMEPLAY_TAG(TAG_Active,"Status.Active")
UE_DEFINE_GAMEPLAY_TAG(TAG_NestTP,"Action.Teleport.Nest")

void AB04_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	MyCageBirdPawn = Cast<AB04_CageBird>(InPawn);
	if (!IsValid(MyCageBirdPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("OnPossess: Invalid Pawn need B04"));
		return;
	}
	
	UAbilitySystemComponent* ASC = MyCageBirdPawn->GetAbilitySystemComponent();

	

	FOnGameplayEffectTagCountChanged& ChargeDelegate = ASC->RegisterGameplayTagEvent(TAG_Charged, EGameplayTagEventType::NewOrRemoved);
	ChargeDelegate.AddUObject(this, &AB04_Controller::ActorCharged);

	FOnGameplayEffectTagCountChanged& FreeDelegate = ASC->RegisterGameplayTagEvent(TAG_FreeMove, EGameplayTagEventType::NewOrRemoved);
	FreeDelegate.AddUObject(this, &AB04_Controller::ActorFree);

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
{
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_VerifyOptimizationTick,
		this,
		&AB04_Controller::VerifyAndFixOptimizationTick,
		3.0f,
		false
	);
}));
	
}

void AB04_Controller::HandleSight(bool IsSpotted, AActor* Actor)
{
	
	ActorSpotted(Actor, GetTeamAttitudeTowards(*Actor));
	if (IsSpotted && Player == nullptr)
	{
		AMainCharacter* VievedActor  = Cast<AMainCharacter>(Actor);
		if (IsValid(VievedActor))
		{
			Player = VievedActor;
			Player->GetLifeComponent()->OnDamageTaken.AddDynamic(this, &AB04_Controller::OnDamageTakenHandler);
			MyCageBirdPawn->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_Spotted),true);

			if (IsValid(GetBlackboardComponent()))
			{
				GetBlackboardComponent()->SetValueAsObject(KeyNameHasPlayer, Actor);
			}
		}
	}
}

bool AB04_Controller::TryActivation()
{
	UAbilitySystemComponent* ASC = MyCageBirdPawn->GetAbilitySystemComponent();
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (ASC->HasMatchingGameplayTag(TAG_FreeMove) && ASC->HasMatchingGameplayTag(TAG_Charged) )
	{
		BirdSetActive();
		
		AIPerceptionComponent->SetSenseEnabled(SightConfig->GetSenseImplementation(),true);
		ASC->AddLooseGameplayTag(TAG_Active);
		return true;
	}

	AIPerceptionComponent->SetSenseEnabled(SightConfig->GetSenseImplementation(),false);
	
	ASC->RemoveLooseGameplayTag(TAG_Active);
	
	if (IsValid(Player))
	{
		Player->GetLifeComponent()->OnDamageTaken.RemoveDynamic(this, &AB04_Controller::OnDamageTakenHandler);
		Player = nullptr;
	}
	if (BB)
	{
		BB->ClearValue(KeyNameHasPlayer);
	}
	BirdSetInactive();
	
	return false;
}

void AB04_Controller::AssignNestPosition(FVector Position)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	NestPosition = Position;
	if (BB)
	{
		BB->SetValueAsVector(KeyNameNestPosition, Position);
	}
}

void AB04_Controller::AssignAltarPosition(const FTransform& Transform, bool SetOnAltar)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	AltarTransform = Transform;
	if (BB && !SetOnAltar)
	{
		BB->SetValueAsVector(KeyNameAltarPosition, Transform.GetLocation());
	}
	if (SetOnAltar)
	{
		MyCageBirdPawn->Teleport(Transform.GetLocation(),MOVE_Walking);
		MyCageBirdPawn->SetActorRotation(Transform.GetRotation());
		MyCageBirdPawn->SelfCharge();
		MyCageBirdPawn->SetFree(false);
		TryActivation();
	}
}

void AB04_Controller::ReachedAltar()
{
	MyCageBirdPawn->SetFree(false);
	OnReachedAltar.Broadcast();
}

void AB04_Controller::ActorCharged(const FGameplayTag Tag, int32 NewCount)
{

	if (NewCount > 0)
	{
		OnActorCharged.Broadcast(true);
	}
	else
	{
		OnActorCharged.Broadcast(false);
	}
	TryActivation();
}

void AB04_Controller::ActorFree(const FGameplayTag Tag, int32 NewCount)
{
	
	if (NewCount > 0)
	{
		OnActorFree.Broadcast(true);
	}
	else
	{
		OnActorFree.Broadcast(false);
	}
	TryActivation();
}



void AB04_Controller::OnDamageTakenHandler(USceneComponent* ThisComponent, float Damage)
{
	UAbilitySystemComponent* ASC = MyCageBirdPawn->GetAbilitySystemComponent();
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (IsValid(ASC))
	{
		FGameplayTagContainer Tag_Tp(TAG_NestTP);
		ASC->TryActivateAbilitiesByTag(Tag_Tp,true);
		UE_LOG(LogTemp,Warning,TEXT("teleport nest"));
	}
	//clean blackboard value 
	if (IsValid(BB))
	{
		BB->ClearValue(KeyNameHasPlayer);
		UE_LOG(LogTemp,Warning,TEXT("teleport nest BB reset"));
		BB->ClearValue(KeyNameAltarPosition);
	}
	Player->GetLifeComponent()->OnDamageTaken.RemoveDynamic(this, &AB04_Controller::OnDamageTakenHandler);
	Player = nullptr;
}

void AB04_Controller::VerifyAndFixOptimizationTick()
{
	
	if (!MyCageBirdPawn)
	{
		return;
	}
	GetPathFollowingComponent()->SetComponentTickEnabled(false);
		
}
