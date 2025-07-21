#include "Journal/BestiaryPageWidget.h"

#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

bool UBestiaryPageWidget::Initialize()
{
	for (const TPair<UImage*, UMaterialInterface*>& Pair : BlockerMaterials)
	{
		UImage* Blocker = Pair.Key;
		UMaterialInterface* BaseMaterial = Pair.Value;

		if (!Blocker || !BaseMaterial)
			continue;

		UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		DynamicMat->SetScalarParameterValue(FName("CanFade"), 0);
		DynamicMat -> SetScalarParameterValue(FName("AlreadyFaded"), 1);
		DynamicMat->SetScalarParameterValue(FName("CurrentTime"), FApp::GetCurrentTime() - GStartTime);
		Blocker->SetBrushFromMaterial(DynamicMat);
	}
	return Super::Initialize();
}

void UBestiaryPageWidget::ResetBlockers(UImage* Blocker)
{
	UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(Blocker->GetDynamicMaterial());
	DynMat -> SetScalarParameterValue(FName("AlreadyFaded"), 1);
	DynMat->SetScalarParameterValue(FName("CanFade"), 0);
	DynMat->SetScalarParameterValue(FName("CurrentTime"), FApp::GetCurrentTime() - GStartTime);
}

void UBestiaryPageWidget::LerpMaterial(UImage* Blocker)
{
	if (!Blocker)
	{
		UE_LOG(LogTemp, Warning, TEXT("!Blocker"));
		return;
	}

	if (!BlockersFading.Contains(Blocker))
	{
		UE_LOG(LogTemp, Warning, TEXT("Blocker Added %s"), *Blocker->GetName());
		BlockersFading.Add(Blocker);
		UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(Blocker->GetDynamicMaterial());

		DynMat -> SetScalarParameterValue(FName("AlreadyFaded"), 1);
		DynMat->SetScalarParameterValue(FName("CanFade"), 1);
		DynMat->SetScalarParameterValue(FName("CurrentTime"), FApp::GetCurrentTime() - GStartTime);
	}
}

void UBestiaryPageWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UBestiaryPageWidget::CompleteAllFades()
{
	if (BlockersFading.Num() == 0)
		return;
	
	for (UImage* Blocker: BlockersFading)
	{
		if (!IsValid(Blocker))
			continue;
		if (UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(Blocker->GetDynamicMaterial()))
		{
			DynMat->SetScalarParameterValue(FName("CanFade"), 0);
			DynMat -> SetScalarParameterValue(FName("AlreadyFaded"), 1);
			DynMat->SetScalarParameterValue(FName("CurrentTime"), FApp::GetCurrentTime() - GStartTime);

			Blocker->SetBrushFromMaterial(DynMat);
		}
	}
	BlockersFading.Empty();
}

void UBestiaryPageWidget::ResetCreatureSelected()
{
	CreatureSelected = FName("Creature");
}

void UBestiaryPageWidget::InitializeScribbles()
{
	if (BorderNoteLeft != nullptr)
	{
		BorderNoteLeft -> SetRenderOpacity(0);
	}
	if (BorderNoteRight != nullptr)
	{
		BorderNoteRight -> SetRenderOpacity(0);
	}
	if (BorderTitle != nullptr)
	{
		BorderTitle -> SetRenderOpacity(0);
	}
	if (BorderLore != nullptr)
	{
		BorderLore -> SetRenderOpacity(0);
	}
	if (Scribble1 != nullptr)
	{
		//Page -> Scribble1 -> SetVisibility(ESlateVisibility::Collapsed);
		Scribble1 -> SetRenderOpacity(0);
	}
	if (Scribble2 != nullptr)
	{
		//Page -> Scribble2 -> SetVisibility(ESlateVisibility::Collapsed);
		Scribble2 -> SetRenderOpacity(0);
	}
	if (Scribble3 != nullptr)
	{
		//Page -> Scribble3 -> SetVisibility(ESlateVisibility::Collapsed);
		Scribble3 -> SetRenderOpacity(0);
	}
	if (Scribble4 != nullptr)
	{
		//Page -> Scribble4 -> SetVisibility(ESlateVisibility::Collapsed);
		Scribble4 -> SetRenderOpacity(0);
	}
	if (Scribble5 != nullptr)
	{
		//Page -> Scribble4 -> SetVisibility(ESlateVisibility::Collapsed);
		Scribble5 -> SetRenderOpacity(0);
	}
	if (Scribble6 != nullptr)
	{
		//Page -> Scribble4 -> SetVisibility(ESlateVisibility::Collapsed);
		Scribble6 -> SetRenderOpacity(0);
	}
	if (Scribble7 != nullptr)
	{
		//Page -> Scribble4 -> SetVisibility(ESlateVisibility::Collapsed);
		Scribble7 -> SetRenderOpacity(0);
	}
	if (Scribble8 != nullptr)
	{
		//Page -> Scribble4 -> SetVisibility(ESlateVisibility::Collapsed);
		Scribble8 -> SetRenderOpacity(0);
	}
	if (TemperIcon != nullptr)
	{
		TemperIcon -> SetRenderOpacity(0);
	}
	/*if (EntryImage != nullptr)
	{
		EntryImage -> SetRenderOpacity(0);
	}*/
}

