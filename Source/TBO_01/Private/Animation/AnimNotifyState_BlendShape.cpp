#include "Animation/AnimNotifyState_BlendShape.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_BlendShape::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!IsValid(MeshComp))
		return;

	FBlendShapeInfo Info;
	Info.Duration = TotalDuration;
	Info.StartTime = MeshComp->GetWorld()->GetTimeSeconds();

	InstanceMap.Add(MeshComp->GetUniqueID(), Info);
}

void UAnimNotifyState_BlendShape::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!IsValid(MeshComp))
		return;

	const uint32 Key = MeshComp->GetUniqueID();
	FBlendShapeInfo* Info = InstanceMap.Find(Key);
	if (!Info)
		return;

	float Elapsed = MeshComp->GetWorld()->GetTimeSeconds() - Info->StartTime;
	float Alpha = FMath::Clamp(Elapsed / Info->Duration, 0.f, 1.f);

	float MorphValue = FMath::Lerp(StartValue, EndValue, Alpha);
	MeshComp->SetMorphTarget(MorphTargetName, MorphValue);
}

void UAnimNotifyState_BlendShape::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!IsValid(MeshComp))
		return;
	
	MeshComp->SetMorphTarget(MorphTargetName, EndValue);

	InstanceMap.Remove(MeshComp->GetUniqueID());
}
