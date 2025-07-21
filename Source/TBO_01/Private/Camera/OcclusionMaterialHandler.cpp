// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/OcclusionMaterialHandler.h"

#include "Camera/GameCamera.h"
#include "Materials/MaterialParameterCollectionInstance.h"


void UOcclusionMaterialHandler::InitializeOcclusionMaterial(AGameCamera* ParentCamera)
{
	if (!OcclusionMPC)
		return;

	GameCamera = ParentCamera;
	OcclusionMPCInstance = GetWorld()->GetParameterCollectionInstance(OcclusionMPC);
	OcclusionMPCInstance->GetScalarParameterValue(FName("Radius"), OcclusionRadius);
	
	SetOcclusionDistance(GameCamera->LenghtModifier->GetTargetArmLenght());
	SetOcclusionOffset(FVector::ZeroVector);
	
	GameCamera->LenghtModifier->OnLengthUpdate.AddDynamic(this, &ThisClass::SetOcclusionDistance);
	GameCamera->OffsetHandlerModifier->OnOffsetUpdate.AddDynamic(this, &ThisClass::SetOcclusionOffset);
}

void UOcclusionMaterialHandler::SetOcclusionDistance(float TargetArmLength)
{
	if (!OcclusionMPCInstance)
		return;

	CurrentTargetLength = TargetArmLength - OcclusionRadius - AdditionalDistanceCorrection;
	OcclusionMPCInstance->SetScalarParameterValue(FName("Distance"), CurrentTargetLength);
}

void UOcclusionMaterialHandler::SetOcclusionOffset(FVector TargetOffset)
{
	if (!OcclusionMPCInstance)
		return;
	
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GameCamera->GetAttachedActor());

	const bool ThereIsObstacle = GetWorld()->LineTraceSingleByChannel(HitResult,
	                                                                  GameCamera->GetCameraComponent()->GetComponentLocation(),
	                                                                  GameCamera->GetAttachedActor()->GetActorLocation(),
	                                                                  ECC_Visibility,
	                                                                  TraceParams);
	
	if (ThereIsObstacle)
		OcclusionMPCInstance->SetScalarParameterValue(FName("Distance"),  CurrentTargetLength + BehindNearObstacleDistanceCorrection);
	else
		OcclusionMPCInstance->SetScalarParameterValue(FName("Distance"),  CurrentTargetLength);
	
	OcclusionMPCInstance->SetVectorParameterValue(FName("OcclusionOffset"),
									FLinearColor(-TargetOffset.X * OffsetCorrection,
															 -TargetOffset.Y * OffsetCorrection,
															 -TargetOffset.Z * OffsetCorrection,
														 	 0.0f));
}
