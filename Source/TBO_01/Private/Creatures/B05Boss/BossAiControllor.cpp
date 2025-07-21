// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/B05Boss/BossAiControllor.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Lifesystem/LifeSystemComponent.h"

bool ABossAiControllor::SetPlayerTarget()
{
	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Blackboard || !IsValid(Player))
	{
		return false;
	}

	Blackboard->SetValueAsObject(KeyNameHasPlayer, Player);
	Player->GetLifeComponent()->OnDeath.AddDynamic(this, &ABossAiControllor::PlayerDeath);
	return true;
}


void ABossAiControllor::PlayerDeath(USceneComponent* PlayerLifeComponent)
{
	Cast<ULifeSystemComponent>(PlayerLifeComponent)->OnDeath.RemoveDynamic(this, &ABossAiControllor::PlayerDeath);

	if (!Blackboard)
	{
		return;
	}
	Blackboard->ClearValue(KeyNameHasPlayer);
	
}
