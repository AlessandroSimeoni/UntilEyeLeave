// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Senses/ComplexCreatureSight.h"
#include "DrawDebugHelpers.h"
#include "ComponentUtils.h"
#include "VectorTypes.h"

// Sets default values for this component's properties
UComplexCreatureSight::UComplexCreatureSight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UComplexCreatureSight::BeginPlay()
{
	Super::BeginPlay();

	
	for (int32 i = 0; i < FieldOfViews.Num(); i++)
	{
		FieldOfViews[i].RadiansAngle = FMath::Cos(FMath::DegreesToRadians(FieldOfViews[i].DegreeAngle/2));
	}
}

void UComplexCreatureSight::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	
	for (FActorSpotted& Actor : SpottedActors)
	{
		if (Actor.TimerHandleExitCone.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(Actor.TimerHandleExitCone);
		}

		if (Actor.TimerHandleExitLineSightSense.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(Actor.TimerHandleExitLineSightSense);
		}
	}
	
	SpottedActors.Empty();
	CurrentPositionInCheck = 0;

//	UE_LOG(LogTemp, Warning, TEXT("UComplexCreatureSight - Cleanup complete"));
}



//handle the different field off view on separate tick
void UComplexCreatureSight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (!IsValid(this) || !GetWorld() || !Pawn || Pawn->IsPendingKillPending())
	{
		return;
	}

	//CleanupInvalidActors();  

	if (FieldOfViews.Num() == 0 || !Pawn)
		return;

	if (DrawDebug)
	{
		DrawDebugVisionCone();
	}
	
	//no actor to check
	if (SpottedActors.Num() == 0 || !SpottedActors.IsValidIndex(CurrentPositionInCheck))
	{
		CurrentPositionInCheck = 0;
		return;
	}
	
	
	//split the check between the frame
	FActorSpotted& ActorToCheck = SpottedActors[CurrentPositionInCheck];

	if (!ActorToCheck.Actor.IsValid()) 
	{
		SpottedActors.RemoveAt(CurrentPositionInCheck);
		CurrentPositionInCheck = 0;
		return;
	}
	
	//check all field of view for the current spotted actor 
	for (int32 i = 0; i < FieldOfViews.Num(); i++)
	{
	
		if (CheckActorSpotted(FieldOfViews[i],ActorToCheck))
		{
			SetTargetSpotted(true, ActorToCheck);
			GetWorld()->GetTimerManager().ClearTimer(ActorToCheck.TimerHandleExitCone);
			continue;
		}

		if (i == SpottedActors.Num() && ActorToCheck.IsSpotted && !ActorToCheck.TimerHandleExitCone.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(ActorToCheck.TimerHandleExitCone, [this, &ActorToCheck]()
			{
				SetTargetSpotted(false, ActorToCheck);
			}, LooseSightTime, false);
		}
	}
	
	//reset from 0 the actor to check
	if (CurrentPositionInCheck >= SpottedActors.Num()-1 )
	{
		CurrentPositionInCheck = 0;
		
	}
	else
	{
		CurrentPositionInCheck++;
	}
}



void UComplexCreatureSight::AddSpottedActor(AActor* Actor)
{
	if (!Actor) return;

	FActorSpotted* ExistingActor = SpottedActors.FindByPredicate([Actor](const FActorSpotted& Spotted)
	{
		return Spotted.Actor == Actor;
	});

	if (ExistingActor)
	{
		//avoid remove if actor reenter line of sight
		if (GetWorld()->GetTimerManager().IsTimerActive(ExistingActor->TimerHandleExitLineSightSense))
		{
			GetWorld()->GetTimerManager().ClearTimer(ExistingActor->TimerHandleExitLineSightSense);
		}
		return;
	}
	
	FActorSpotted SpottedActor;
	SpottedActor.Actor = Actor;
	SpottedActors.Add(SpottedActor);
	CurrentPositionInCheck	= 0;

	
}

void UComplexCreatureSight::RemoveSpottedActor(AActor* Actor)
{
	if (!Actor) return;

	for (int32 i = 0; i < SpottedActors.Num(); i++)
	{
		if (SpottedActors[i].Actor == Actor)
		{
			TWeakObjectPtr<AActor> WeakActor = Actor;  
			FActorSpotted& ActorToRemove = SpottedActors[i];

			GetWorld()->GetTimerManager().SetTimer(ActorToRemove.TimerHandleExitLineSightSense, 
				[this, WeakActor, &ActorToRemove]()
				{
					if (WeakActor.IsValid())  
					{
						ConfirmRemoveActor(ActorToRemove);
					}
				}, 
				LooseSightTime, 
				false
			);
		}
	}
}
void UComplexCreatureSight::ConfirmRemoveActor(FActorSpotted& ActorSpotted)
{
	if (!IsValid(this) || !GetWorld() || !ActorSpotted.Actor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfirmRemoveActor() - Invalid component or actor detected."));
		return;
	}
	if (ActorSpotted.TimerHandleExitCone.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ActorSpotted.TimerHandleExitCone);
	}

	if (ActorSpotted.TimerHandleExitLineSightSense.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ActorSpotted.TimerHandleExitLineSightSense);
	}

	if (ActorSpotted.IsSpotted)
	{
		SetTargetSpotted(false, ActorSpotted);
	}

	int32 IndexToRemove = SpottedActors.IndexOfByPredicate([&](const FActorSpotted& Item)
	{
		return Item.Actor == ActorSpotted.Actor && Item.Actor.IsValid();
	});


	if (IndexToRemove != INDEX_NONE)
	{
		SpottedActors.RemoveAt(IndexToRemove);
	}

	CurrentPositionInCheck = 0;
}

void UComplexCreatureSight::RefreshSpottedActor(AActor* Actor)
{
	if (!Actor)return;

	for (int32 i = 0; i < SpottedActors.Num(); i++)
	{
		if (SpottedActors[i].Actor == Actor)
		{
			SpottedActors[i].IsSpotted = false;
			return;
		}
		
	}
}




bool UComplexCreatureSight::CheckActorSpotted(FSightFieldOfView View, FActorSpotted ActorToCheck)
{
	if (!IsValid(this) || !GetWorld() || !Pawn || Pawn->IsPendingKillPending())
	{
		UE_LOG(LogTemp, Error, TEXT("CheckActorSpotted - Component or Pawn is invalid or pending kill"));
		return false;
	}

	if (!ActorToCheck.Actor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckActorSpotted - ActorToCheck is invalid or pending kill"));
		return false;
	}
	FVector SelfPostion =  Pawn->GetActorLocation();
	FVector SpottedActorPosition = ActorToCheck.Actor->GetActorLocation();
	FVector ForwardVector = Pawn->GetActorForwardVector();

	FVector DirectionToTarget = (SpottedActorPosition - SelfPostion).GetSafeNormal();

	float DotProduct = FVector::DotProduct(DirectionToTarget, ForwardVector);

	if (DotProduct < View.RadiansAngle)
	{
		return false;
	}

	float DistSquared = FVector::DistSquared(SelfPostion, SpottedActorPosition);
	
	//check if inside the view
	if (DistSquared <= View.Distance * View.Distance)
	{
		return true;
	}

	
	//if the actor is outside the normal distance but is spotted end inside the normal distance still return true
	if (ActorToCheck.IsSpotted && DistSquared <= View.LoseDistance * View.LoseDistance)
	{
		return true;
	}
	return  false;
}

void UComplexCreatureSight::SetTargetSpotted(bool IsSpotted, FActorSpotted& SpottedActor)
{
	if (IsSpotted == SpottedActor.IsSpotted)
		return;

	SpottedActor.IsSpotted = IsSpotted;

	if (!IsSpotted && GetWorld())
	{
		if (SpottedActor.TimerHandleExitCone.IsValid()) 
		{
			GetWorld()->GetTimerManager().ClearTimer(SpottedActor.TimerHandleExitCone);
		}
	}

	// Send event spotted change
	if (OnTargetSpottedChanged.IsBound())
	{
		OnTargetSpottedChanged.ExecuteIfBound(IsSpotted, SpottedActor);
	}
}


void UComplexCreatureSight::SetPawn(APawn* ControlledPawn)
{
	if (ControlledPawn)
	{
		Pawn = ControlledPawn;
	}
}


void UComplexCreatureSight::DrawDebugVisionCone()
{
    AActor* Owner = Pawn;
    if (!Owner) return;

    FVector SelfPosition = Owner->GetActorLocation();
    FVector ForwardVector = Owner->GetActorForwardVector();

    int32 NumSegments = 10; 

    for (const FSightFieldOfView& View : FieldOfViews)
    {
        float HalfAngle = View.DegreeAngle / 2.0f;

        
        TArray<FVector> VisionPoints;
        for (int32 i = 0; i <= NumSegments; i++)
        {
            float Angle = -HalfAngle + (i / (float)NumSegments) * View.DegreeAngle;
            FVector RotatedVector = ForwardVector.RotateAngleAxis(Angle, FVector::UpVector);
            VisionPoints.Add(SelfPosition + (RotatedVector * View.Distance));
        }

        
        for (int32 i = 0; i < VisionPoints.Num() - 1; i++)
        {
            DrawDebugLine(GetWorld(), VisionPoints[i], VisionPoints[i + 1], View.DebugColor, false, 0.05f, 0, 1.5f);
        }

       
        DrawDebugLine(GetWorld(), SelfPosition, VisionPoints[0], View.DebugColor, false, 0.05f, 0, 1.5f);
        DrawDebugLine(GetWorld(), SelfPosition, VisionPoints.Last(), View.DebugColor, false, 0.05f, 0, 1.5f);

        
        TArray<FVector> LoseVisionPoints;
        for (int32 i = 0; i <= NumSegments; i++)
        {
            float Angle = -HalfAngle + (i / (float)NumSegments) * View.DegreeAngle;
            FVector RotatedVector = ForwardVector.RotateAngleAxis(Angle, FVector::UpVector);
            LoseVisionPoints.Add(SelfPosition + (RotatedVector * View.LoseDistance));
        }

        
        for (int32 i = 0; i < LoseVisionPoints.Num() - 1; i++)
        {
            DrawDebugLine(GetWorld(), LoseVisionPoints[i], LoseVisionPoints[i + 1], View.DebugColor.WithAlpha(100), false, 0.05f, 0, 1.5f);
        }

        
        DrawDebugLine(GetWorld(), SelfPosition, LoseVisionPoints[0], View.DebugColor.WithAlpha(100), false, 0.05f, 0, 1.5f);
        DrawDebugLine(GetWorld(), SelfPosition, LoseVisionPoints.Last(), View.DebugColor.WithAlpha(100), false, 0.05f, 0, 1.5f);
    }
	
}

void UComplexCreatureSight::CleanupInvalidActors()
{
	//to check if impact performance
	SpottedActors.RemoveAll([](const FActorSpotted& SpottedActor)
	{
		return !SpottedActor.Actor.IsValid();
	});
	
}