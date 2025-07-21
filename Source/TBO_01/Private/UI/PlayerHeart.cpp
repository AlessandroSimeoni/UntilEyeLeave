// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHeart.h"

#include "Components/Image.h"

void UPlayerHeart::FillHeartByValue(float Value)
{
	RequestedValue = Value;
	UpdateHeart(true);
}

void UPlayerHeart::PlayFillHeartAnim(float Value)
{
	RequestedValue = Value;
	PlayAnimation(HealAnimation);
}

void UPlayerHeart::EmptyHeartByValue(float Value)
{
	RequestedValue = Value;
	UpdateHeart(false);
}

void UPlayerHeart::PlayEmptyHeartAnim(float Value)
{
	RequestedValue = Value;
	PlayAnimation(DamageAnimation);
}

void UPlayerHeart::PlayHeartCreationAnim(float Value)
{
	const float ClampedValue = FMath::Clamp(Value, 0.0f, 1.0f);
	
	if (ClampedValue == 1.0f)
		HeartImage->SetBrushFromTexture(FullHeartSprite);
	else if (ClampedValue > 0.0f && ClampedValue < 1.0f)
		HeartImage->SetBrushFromTexture(HalfHeartSprite);
	else
		HeartImage->SetBrushFromTexture(EmptyHeartSprite);
	
	PlayAnimation(CreationAnimation);
}

void UPlayerHeart::PlayHeartDestructionAnim()
{
	PlayAnimation(DestructionAnimation);
}

void UPlayerHeart::BroadcastCreationOrDestruction(const bool IsCreation)
{
	OnHeartCreatedOrDestroyed.Broadcast(this, IsCreation);
}

void UPlayerHeart::UpdateHeart(bool IsHeal) const
{
	if (RequestedValue < 0.0f || RequestedValue > 1.0f)
		return;

	if (RequestedValue == 1.0f)
		HeartImage->SetBrushFromTexture(IsHeal ? FullHeartSprite : EmptyHeartSprite);
	else
		HeartImage->SetBrushFromTexture(HalfHeartSprite);

	if (IsHeal)
		OnHeartFilled.Broadcast();
	else
		OnHeartEmptied.Broadcast();
}


