// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHealthBar.h"

#include "Character/MainCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Lifesystem/LifeSystemComponent.h"
#include "UI/PlayerHeart.h"

void UPlayerHealthBar::InitHealthBar(AMainCharacter* Character)
{
	if (!Character)
		return;
	
	MainCharacter = Character;
	MainCharacterLife = MainCharacter->GetLifeComponent();
	MainCharacterLife->OnDeath.AddDynamic(this, &ThisClass::EmptyTotalHealthBar);
	MainCharacterLife->OnDamageTaken.AddDynamic(this, &ThisClass::EmptyHealthBar);
	MainCharacterLife->OnHealReceived.AddDynamic(this, &ThisClass::FillHealthBar);
	
	for (int i = 0; i < MainCharacterLife->MaxHealth; i++)
	{
		UPlayerHeart* NewHeartWidget = CreateWidget<UPlayerHeart>(GetWorld(), HeartWidgetClass);
		if (NewHeartWidget)
		{
			NewHeartWidget->AddToViewport();
			UHorizontalBoxSlot* HeartSlot = Cast<UHorizontalBoxSlot>(HeartContainer->AddChild(NewHeartWidget));
			if (HeartSlot)
				HeartSlot->SetPadding(FMargin(2.0f));

			Hearts.Add(NewHeartWidget);
			CurrentHeartIndex++;
		}
	}
}

void UPlayerHealthBar::EmptyHealthBar(USceneComponent* InstigatorComponent, float Damage)
{
	// if no hp left then the EmptyTotalHealthBar will be called, no need to execute this
	if (MainCharacterLife->GetCurrentHealth() <= 0.0f)
		return;

	WasFullyHealed = false;
	
	const float ResidualHealth = MainCharacterLife->GetCurrentHealth();
	const int TargetIndex = FMath::FloorToInt(ResidualHealth);
	const bool IsDecimal = ResidualHealth != TargetIndex;

	for (int i = CurrentHeartIndex; i >= TargetIndex; i--)
	{
		if (IsDecimal)
		{
			if(i > TargetIndex)
				Hearts[i]->PlayEmptyHeartAnim(1.0f);
			else
				Hearts[i]->PlayEmptyHeartAnim(0.5f);
		}
		else
			Hearts[i]->PlayEmptyHeartAnim(1.0f);
	}

	if (IsDecimal)
		CurrentHeartIndex = TargetIndex;
	else
		CurrentHeartIndex = TargetIndex - 1;
	
	OnHealthBarEmptied.Broadcast();
}

void UPlayerHealthBar::EmptyTotalHealthBar(USceneComponent* InstigatorComponent)
{
	for (int i = CurrentHeartIndex; i >= 0; i--)
		Hearts[i]->PlayEmptyHeartAnim(1.0f);

	CurrentHeartIndex = -1;
	
	OnCompletelyEmptyHealthBar.Broadcast();
}

void UPlayerHealthBar::FillHealthBar(USceneComponent* InstigatorComponent, float HealValue)
{
	// if player has already max hp no need to execute this
	if (WasFullyHealed && FMath::CeilToInt(MainCharacterLife->MaxHealth) == CurrentHeartIndex + 1)
		return;
	
	float TargetHealth = MainCharacterLife->GetCurrentHealth();
	int TargetIndex = FMath::FloorToInt(TargetHealth);
	if (TargetIndex > Hearts.Num() || (TargetIndex == Hearts.Num() && TargetHealth > TargetIndex))
	{
		TargetIndex = Hearts.Num();
		TargetHealth = TargetIndex;
	}
	const bool IsDecimal = TargetHealth != TargetIndex;

	if (CurrentHeartIndex < 0)
		CurrentHeartIndex = 0;

	const float PreHealingHealth = TargetHealth - HealValue;
	if (PreHealingHealth > 0.0f && CurrentHeartIndex < Hearts.Num() - 1 && PreHealingHealth - FMath::FloorToInt(PreHealingHealth) == 0.0f)
		CurrentHeartIndex++;
	
	for (int i = CurrentHeartIndex; i <= (IsDecimal ? TargetIndex : TargetIndex - 1); i++)
	{
		if (IsDecimal)
		{
			if(i < TargetIndex)
				Hearts[i]->PlayFillHeartAnim(1.0f);
			else
				Hearts[i]->PlayFillHeartAnim(0.5f);
		}
		else
		{
			Hearts[i]->PlayFillHeartAnim(1.0f);
			if (i == Hearts.Num() - 1)
				WasFullyHealed = true;
		}
	}

	if (IsDecimal)
		CurrentHeartIndex = TargetIndex;
	else
		CurrentHeartIndex = TargetIndex - 1;
	
	OnHealthBarFilled.Broadcast();
}

void UPlayerHealthBar::ResizeHealthBar()
{
	int DeltaHearts = FMath::CeilToInt(MainCharacterLife->MaxHealth) - Hearts.Num();

	// if the number of hearts already reflects the max health, there's no need to execute this function 
	if (DeltaHearts == 0)
		return;
	
	if (DeltaHearts < 0)
	{
		//remove hearts
		int TargetIndex = Hearts.Num() - 1 + DeltaHearts;
		for (int i = Hearts.Num() - 1; i > TargetIndex; i--)
		{
			Hearts[i]->OnHeartCreatedOrDestroyed.AddDynamic(this, &ThisClass::HandleHeartCreatedOrDestroyed);
			Hearts[i]->PlayHeartDestructionAnim();
		}
	}
	else
	{
		// add hearts
		for (int i = 0; i < DeltaHearts; i++)
		{
			UPlayerHeart* NewHeartWidget = CreateWidget<UPlayerHeart>(GetWorld(), HeartWidgetClass);
			if (NewHeartWidget)
			{
				float EmptyOrFullHeart = FMath::Clamp(MainCharacterLife->GetCurrentHealth()-Hearts.Num(), 0.0f, 1.0f);
				NewHeartWidget->PlayHeartCreationAnim(EmptyOrFullHeart);
				
				if (EmptyOrFullHeart > 0.0f)
					CurrentHeartIndex++;
				
				NewHeartWidget->AddToViewport();
				UHorizontalBoxSlot* HeartSlot = Cast<UHorizontalBoxSlot>(HeartContainer->AddChild(NewHeartWidget));
				if (HeartSlot)
					HeartSlot->SetPadding(FMargin(2.0f));

				Hearts.Add(NewHeartWidget);
			}
		}
	}
	
}

void UPlayerHealthBar::HandleHeartCreatedOrDestroyed(UPlayerHeart* TargetHeart, bool IsCreated)
{
	if (!TargetHeart)
		return;
	
	TargetHeart->OnHeartCreatedOrDestroyed.RemoveDynamic(this, &ThisClass::HandleHeartCreatedOrDestroyed);

	if (!IsCreated)
	{
		if (CurrentHeartIndex == Hearts.Num() - 1)
			CurrentHeartIndex--;
		
		// remove target heart from health bar
		HeartContainer->RemoveChild(TargetHeart);
		Hearts.RemoveSingle(TargetHeart);
		TargetHeart->RemoveFromParent();
		TargetHeart->ConditionalBeginDestroy();
	}
}



