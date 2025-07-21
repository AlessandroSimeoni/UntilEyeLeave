// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/BehaviorTree/Service/NearScriptedPosition.h"

#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creatures/ScriptedController.h"
#include "Creatures/Patrol/Interface/PatrolInterface.h"


void UNearScriptedPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	FNearPositionMemory* MyMemory = (FNearPositionMemory*)NodeMemory;


	if (!IsValid(MyMemory->PatrolSplineComponent)) return;

	
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn) return;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	float PositionDistance = FVector::Distance(ControlledPawn->GetActorLocation(), 
												BlackboardComp->GetValueAsVector(Position.SelectedKeyName));
	
	if (PositionDistance < MinDistance && MyMemory->PatrolSplineComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("ValidPatrol"));
		if (MyMemory->PatrolSplineComponent->IncrementIndexAndSendScriptedActionEvent())
		{
			FVector NewPosition = MyMemory->PatrolSplineComponent->GetPosition();
				if (NewPosition.Equals(FVector::ZeroVector))
				{
					UE_LOG(LogTemp,Warning,TEXT("Path 0"));
					AScriptedController* Controller = Cast<AScriptedController>(OwnerComp.GetAIOwner());
					if (IsValid(Controller))
					{
						Controller->bPathCompleted = true;
					}
					BlackboardComp->SetValueAsBool(IsPatrolPathAssigned.SelectedKeyName, false);
					return;
				}
			BlackboardComp->SetValueAsVector(Position.SelectedKeyName, NewPosition);
			DrawDebugPatrol(NewPosition);
		}
	}
}


