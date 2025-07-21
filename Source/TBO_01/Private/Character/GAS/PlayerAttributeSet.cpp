// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GAS/PlayerAttributeSet.h"

#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"


void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		if (Attribute == GetCoinAttribute())
		{
			OnCoinChanged.Broadcast(NewValue,OldValue);
			
			USaveSubsystem* SaveSubsystem = ActorInfo->AvatarActor->GetWorld()->GetGameInstance()->GetSubsystem<USaveSubsystem>();
			if (SaveSubsystem)
			{
				if (UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame())
				{
					TBOSaveGame->CoinsCollected = NewValue;
				}
			}
			
			UE_LOG(LogTemp, Display, TEXT("OnCoinChanged"));
		}
	}
}
