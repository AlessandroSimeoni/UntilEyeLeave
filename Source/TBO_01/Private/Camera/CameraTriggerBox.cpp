#include "Camera/CameraTriggerBox.h"

#include <filesystem>

#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Character/MainCharacter.h"

ACameraTriggerBox::ACameraTriggerBox()
{
    OnActorBeginOverlap.AddDynamic(this, &ACameraTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ACameraTriggerBox::OnOverlapEnd);
}

void ACameraTriggerBox::BeginPlay()
{
    Super::BeginPlay();
}

void ACameraTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor) return;

    AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
    if (IsValid(Character))
    {
        if (IsEnterDirectionEnabled && !IsActorInValidDirection(OtherActor, RequiredEnterAngle))
        {
            return;
        }
            OnCharacterEnter(Character->GameCamera);
    }
}

void ACameraTriggerBox::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor) return;
    AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
    if (IsValid(Character))
    {
        if (IsExitDirectionEnabled && !IsActorInValidDirection(OtherActor, RequiredExitAngle))
        {
            return;
        }
        
        OnCharacterExit(Character->GameCamera);
            
        
    }
}

bool ACameraTriggerBox::IsActorInValidDirection(AActor* Actor, float RequiredAngle ) const
{
    if (!Actor) return false;

    FVector ToActor = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    
    FVector ForwardVector = (RequiredAngle >= 0) ? GetActorForwardVector() : -GetActorForwardVector();
    
    float DotProduct = FVector::DotProduct(ForwardVector, ToActor);
    float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f)));

   if (DrawDebug)
       DebugAngle(ForwardVector,ToActor,RequiredAngle);

    
    return AngleDegrees <=  FMath::Abs(RequiredAngle);;
}

void ACameraTriggerBox::DebugAngle(FVector ForwardVector, FVector ToActor,float RequiredAngle)const
{
  

    FVector LeftBoundary = ForwardVector.RotateAngleAxis(-RequiredAngle, FVector::UpVector);
    FVector RightBoundary = ForwardVector.RotateAngleAxis(RequiredAngle, FVector::UpVector);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + ForwardVector * DebugLineLength, FColor::Green, false, 3.0f, 0, 2.0f);
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + ToActor * DebugLineLength, FColor::Blue, false, 3.0f, 0, 2.0f);

    
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + LeftBoundary * DebugLineLength, FColor::Red, false, 3.0f, 0, 2.0f);
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + RightBoundary * DebugLineLength, FColor::Red, false, 3.0f, 0, 2.0f);
    
}