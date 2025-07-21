// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/OffsetHandlerModifier.h"


UOffsetHandlerModifier* UOffsetHandlerModifier::Create(UObject* Outer, AActor* MainActor, float LockOffset,
	float ForwardOffset, float InterpSpeed,float MinDistance,float MaxDistance)
{
	TObjectPtr<UOffsetHandlerModifier> Modifier = NewObject<UOffsetHandlerModifier>(Outer);

	
	//create the 2 modifier to handle
	Modifier->OffsetForwardModifier = UOffsetToForwardModifier::Create(Modifier,MainActor,ForwardOffset,InterpSpeed);
	Modifier->LockOnModifier = ULockOnModifier::Create(Modifier,MainActor,nullptr,LockOffset,InterpSpeed, MinDistance, MaxDistance);
	Modifier->InterpSpeed = InterpSpeed;
	
	return Modifier;
}

void UOffsetHandlerModifier::Update(float DeltaTime, USpringArmComponent* SpringArm)
{
	if (!Enabled)
	{
		if (ImmediateStatus)
		{
			SpringArm->TargetOffset = FVector::Zero();
			ImmediateStatus = false;
		}
		
		SpringArm->TargetOffset = FMath::VInterpTo(SpringArm->TargetOffset,FVector::Zero(),DeltaTime,InterpSpeed);
		OnOffsetUpdate.Broadcast(SpringArm->TargetOffset);
		return;
	}
	if (IsDefaultOffset)
	{
		SpringArm->TargetOffset = FMath::VInterpTo(SpringArm->TargetOffset, DefaultOffset, DeltaTime, InterpSpeed);
		OnOffsetUpdate.Broadcast(SpringArm->TargetOffset);
		return;
	}
	if (!LockOnModifier->TargetIsNull())
	{
		LockOnModifier->Update(DeltaTime,SpringArm);
		return;
	}
	if (!OffsetForwardModifier->TargetIsNull())
	{
		OffsetForwardModifier->Update(DeltaTime,SpringArm);
		OnOffsetUpdate.Broadcast(SpringArm->TargetOffset);
		return;
	}
}

void UOffsetHandlerModifier::Revert(bool Immediate)
{
	OffsetForwardModifier->SetTargetActor(nullptr);
	Enabled = !Enabled;
	ImmediateStatus = Immediate;
}

bool UOffsetHandlerModifier::IsDestructible() const
{
	return false;
}


void UOffsetHandlerModifier::SetStatus(bool Status,bool SetToZero)
{
	Enabled = Status;
	IsDefaultOffset = false;
	if (!Status)
		ImmediateStatus = SetToZero;
}
