#include "Journal/CheckpointMapButton.h"

#include "Character/MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Image.h"
#include "Journal/ConfirmationModalWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WorldActors/Checkpoint.h"

void UCheckpointMapButton::SetIconTexture(UTexture2D* Texture)
{
	if (Icon && Texture)
	{
		Icon -> SetBrushFromTexture(Texture);
	}
}

void UCheckpointMapButton::SetCheckpoint(ACheckpoint* InCheckpoint)
{
	BoundCheckpoint = InCheckpoint;
}

void UCheckpointMapButton::ActualTeleport()
{
	AMainCharacter* MC = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MC -> SetActorLocation(FVector(BoundCheckpoint -> GetRespawnLocation().X, BoundCheckpoint -> GetRespawnLocation().Y, BoundCheckpoint -> GetRespawnLocation().Z + MC -> GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
}

void UCheckpointMapButton::ActivateBorderImage(bool Visible)
{
	if (Visible)
		BorderImage ->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
	{
		BorderImage ->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCheckpointMapButton::BroadcastOnTeleportRequested()
{
	if (BoundCheckpoint)
	{
		OnTeleportRequested.Broadcast(BoundCheckpoint->GetRespawnLocation());
	}
}

