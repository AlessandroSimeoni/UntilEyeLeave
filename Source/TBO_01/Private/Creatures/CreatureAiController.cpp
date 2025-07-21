// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/CreatureAiController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MainCharacter.h"
#include "Creatures/Creature.h"
#include "Creatures/CreatureEnumHelper.h"
#include "Eyes/EyesHandlerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Prediction.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Spotted,"FeedBack.View.Spotted")
UE_DEFINE_GAMEPLAY_TAG(TAG_Lost,"FeedBack.View.Lost")

//const FObjectInitializer& ObjectInitializer
ACreatureAiController::ACreatureAiController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	Cast<UCrowdFollowingComponent>(GetPathFollowingComponent())->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);

	//set up Senses
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	TouchConfig = CreateDefaultSubobject<UAISenseConfig_Touch>(TEXT("TouchConfig"));
	PredictionConfig = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("PredictionConfig"));
	
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->ConfigureSense(*TouchConfig);
	AIPerceptionComponent->ConfigureSense(*PredictionConfig);

	PredictionConfig->SetStartsEnabled(false);
	
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACreatureAiController::OnPerceptionUpdate);

	//set up component
	ComplexCreatureSightComponent = CreateDefaultSubobject<UComplexCreatureSight>(TEXT("ComplexCreatureSight"));

	
}



void ACreatureAiController::BeginPlay()
{
	Super::BeginPlay();
	SetTeam(TeamAffiliationConfig.Team);
	ComplexCreatureSightComponent->OnTargetSpottedChanged.BindUObject(this, &ACreatureAiController::SpottedActorChangeStatus);
	GetPathFollowingComponent()->SetStopMovementOnFinish(false);
	
}

void ACreatureAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ACreature* Creature = Cast<ACreature>(InPawn);
	ComplexCreatureSightComponent->SetPawn(InPawn);
	AbilitySystemComponent = Creature->GetAbilitySystemComponent();
	Creature->CreatureAiController = this;
	
}

void ACreatureAiController::OnUnPossess()
{
	ACreature* Creature = Cast<ACreature>(GetPawn());
	if (Creature)
	{
		Creature->CreatureAiController = nullptr;
	}
	Super::OnUnPossess();
}


void  ACreatureAiController::OnPerceptionUpdate(AActor*  Actor, FAIStimulus  Stimulus)
{
	if (!IsValid(Actor))
		return;
	
	if (Stimulus.Type == SightConfig->GetSenseID())
	{
		HandleSight(Stimulus.WasSuccessfullySensed(), Actor);
		UE_LOG(LogTemp,Warning,TEXT("sight Sense Detected"));
	}
	
	if (Stimulus.Type == HearingConfig->GetSenseID())
	{
		ENoiseAction Noise = UCreatureEnumHelper::FNameToEnum<ENoiseAction>(Stimulus.Tag);
		HandleHearing(Actor,Noise,Stimulus.IsActive());
	}
	if (Stimulus.Type == TouchConfig->GetSenseID())
	{
		HandleTouch(Actor);
		UE_LOG(LogTemp,Warning,TEXT("touch Sense Detected %s"),*this->GetName());
	}
	if (Stimulus.Type == DamageConfig->GetSenseID())
	{
		EDamageAction Damage = UCreatureEnumHelper::FNameToEnum<EDamageAction>(Stimulus.Tag);
		HandleDamage(Actor,Damage,Stimulus.Strength);
		
	}
	if (Stimulus.Type == PredictionConfig->GetSenseID())
	{
		HandlePrediction(Stimulus.StimulusLocation,Actor,Stimulus.IsActive());
		
	}

	
}

void ACreatureAiController::HandleSight(bool IsSpotted, AActor* Actor)
{
	if (IsSpotted)
	{
		ComplexCreatureSightComponent->AddSpottedActor(Actor);
		
	}
	else
	{
		ComplexCreatureSightComponent->RemoveSpottedActor(Actor);
	}
}



void ACreatureAiController::SpottedActorChangeStatus(bool IsSpotted, FActorSpotted SpottedActor)
{
    if (!SpottedActor.Actor.IsValid())  
    {
        UE_LOG(LogTemp, Error, TEXT("SpottedActorChangeStatus() - Actor is invalid or destroyed"));
        return;
    }

    AActor* Actor = SpottedActor.Actor.Get(); 

    
    if (Actor->IsPendingKillPending())
    {
        UE_LOG(LogTemp, Error, TEXT("SpottedActorChangeStatus() - Actor is pending kill"));
        return;
    }

    // If the actor is a creature, take the controller
    const APawn* OtherPawn = Cast<APawn>(Actor);
    ACreatureAiController* CreatureAiController = nullptr;

    if (OtherPawn)
    {
        CreatureAiController = Cast<ACreatureAiController>(OtherPawn->GetController());
    }

    if (IsSpotted)
    {
        ActorSpotted(Actor, GetTeamAttitudeTowards(*Actor));

        if (Cast<AMainCharacter>(Actor))
        {
            GetBlackboardComponent()->SetValueAsObject(KeyNameHasPlayer, Actor);
        	SetEyesTarget(Actor);
        	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_Spotted),true);
        }
        else if (CreatureAiController && IsValid(CreatureAiController) && ComplexCreatureSightComponent)
        {
            if (CreatureAiController->OnSpottedActorChangeAffiliation.IsBound())
            {
                CreatureAiController->OnSpottedActorChangeAffiliation.AddDynamic(
                    ComplexCreatureSightComponent, &UComplexCreatureSight::RefreshSpottedActor);
            }
        }
    }
    else
    {
        ActorLost(Actor);

        if (Cast<AMainCharacter>(Actor))
        {
            UAISense_Prediction::RequestControllerPredictionEvent(this, Actor, 0.5f);
        	RemoveEyesTarget(Actor);
        }
        else if (CreatureAiController && ComplexCreatureSightComponent)
        {
            if (CreatureAiController->OnSpottedActorChangeAffiliation.IsBound())
            {
                CreatureAiController->OnSpottedActorChangeAffiliation.RemoveDynamic(
                    ComplexCreatureSightComponent, &UComplexCreatureSight::RefreshSpottedActor);
            }
        }
    }
}

void ACreatureAiController::HandleHearing_Implementation(AActor * Actor,ENoiseAction NoiseType,bool IsValid)
{
	
	ActorHeared(Actor,NoiseType,IsValid);
}

void ACreatureAiController::HandleTouch_Implementation(AActor* Actor)
{
	if (!Actor) return; 
	if (TouchEventOnlyWithPlayer)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Actor == PlayerPawn)
		{
			ActorTouched(Actor);
			UE_LOG(LogTemp,Warning,TEXT("touch Sense Only Player"));
		}
	}
	else
	{
		ActorTouched(Actor);
		UE_LOG(LogTemp,Warning,TEXT("touch Sense Only Any"));
	}
}

void ACreatureAiController::HandleDamage_Implementation(AActor * Actor,EDamageAction DamageType, float damageValue)
{
	if (!Actor) return;

	UE_LOG(LogTemp, Warning, TEXT("EDamageAction: %s"), *UEnum::GetDisplayValueAsText(EDamageAction::TriggerBoxDamage).ToString());
	if (DamageType == EDamageAction::TriggerBoxDamage)
	{
		TriggerBoxDamage(Actor,DamageType,damageValue);
	}
	else
	{
		ActorDamage(Actor,DamageType,damageValue);
		OnActorDamage.Broadcast(Actor,GetPawn(),DamageType,damageValue);
	}
	
}

void ACreatureAiController::HandlePrediction_Implementation(const FVector &ActorPredictedPosition, AActor * PredictedActor, bool IsValid)
{
	if (!PredictedActor) return;
	if (IsValid)
	{
		GetBlackboardComponent()->SetValueAsVector(KeyNameHasPredictedPosition, ActorPredictedPosition);
		GetBlackboardComponent()->ClearValue(KeyNameHasPlayer);
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_Lost),true);
	}
	
	PredictionActorMovement(ActorPredictedPosition,PredictedActor,IsValid);
}

void ACreatureAiController::SetEyesTarget(AActor* Target)
{
	if (!IsValid(Target) && !IsSetEyeFollow)
	{
		return;
	}

	TScriptInterface<IEyesHandlerInterface> EyesHandlerActor = GetPawn();;
	EyesHandlerActor->Execute_GetEyesHandlerActorComponent(GetPawn())->SetEyesTargetPlayer(Target);
}

void ACreatureAiController::RemoveEyesTarget(AActor* Target)
{
	if (!IsValid(Target) && !IsSetEyeFollow)
	{
		return;
	}

	TScriptInterface<IEyesHandlerInterface> EyesHandlerActor = GetPawn();
	EyesHandlerActor->Execute_GetEyesHandlerActorComponent(GetPawn())->RemoveEyesTargetPlayer(Target);
}

//Affiliation
void ACreatureAiController::SetTeam(EGameTeam Team)
{
	TeamAffiliationConfig.Team = Team;
	SetGenericTeamId(TeamAffiliationConfig.GetTeam());

	if (OnSpottedActorChangeAffiliation.IsBound())
	{
		OnSpottedActorChangeAffiliation.Broadcast(GetPawn());
		SpottedActorChangeAffiliation(GetPawn());
	}
}



ETeamAttitude::Type ACreatureAiController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	//check interface in controller
	if (!OtherTeamAgent) 
	{
		const APawn* OtherPawn = Cast<APawn>(&Other);
		if (OtherPawn)
		{
			const AController* OtherController = OtherPawn->GetController();
			if (OtherController)
			{
				OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherController);
			}
		}
	}
	//No interface return neutral
	if (!OtherTeamAgent)
	{
		return ETeamAttitude::Neutral;
	}

	uint8 TeamIdValue = OtherTeamAgent->GetGenericTeamId().GetId();
	//id not valid for gameTeam Enum
	if (TeamIdValue >= static_cast<uint8>(EGameTeam::Max))
	{
		return ETeamAttitude::Neutral;
	}
	
	EGameTeam OtherTeam = static_cast<EGameTeam>(TeamIdValue);
	return TeamAffiliationConfig.GetTeamAttitude(OtherTeam);

}


