// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/UniqueIDComponent.h"

void UUniqueIDComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (!UniqueID.IsValid())
	{
		UniqueID = FGuid::NewGuid();
	}
}
