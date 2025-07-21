// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"

#include "Character/MainCharacter.h"
#include "UI/PlayerHealthBar.h"
#include "CommonActionWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Journal/CollectableItems.h"
#include "UI/InteractionWithPriceWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (MainCharacter)
	{
		MainCharacter->OnInteractableInRange.AddDynamic(this, &ThisClass::HandleInteractionInRange);
		PC = GetWorld()->GetFirstPlayerController();
		HealthBar->InitHealthBar(MainCharacter);
	}
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!InteractionPromptComponent)
	{
		return;
	}

	FVector2D ScreenPosition;
	if (PC->ProjectWorldLocationToScreen(InteractionPromptComponent->GetComponentLocation(), ScreenPosition))
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		const FVector2D ScaledPosition = ScreenPosition / Scale;

	
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InteractAction->Slot))
		{
			CanvasSlot->SetPosition(ScaledPosition);
		}
	}
}

void UHUDWidget::HandleInteractionInRange(bool Value, USceneComponent* InteractionPromptLocationComponent)
{
	InteractionPromptComponent = (Value ? InteractionPromptLocationComponent : nullptr);

	if (InteractAction)
	{
		InteractAction->SetRenderOpacity(Value ? 1.0f : 0.0f);

		if (InteractAction->ActionWidget)
		{
			InteractAction->ActionWidget->SetRenderOpacity(Value ? 1.0f : 0.0f);
		}
		if (Value == 0)
		{
			InteractAction->SetupInteraction(0);
		}
		else if (ACollectableItems* Item = Cast<ACollectableItems>(InteractionPromptLocationComponent->GetOwner()))
		{
			InteractAction->SetupInteraction(Item->Price);
		}
	}
}
