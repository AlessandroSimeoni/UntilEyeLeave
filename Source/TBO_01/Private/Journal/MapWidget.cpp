#include "Journal/MapWidget.h"
#include "CommonTextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Character/MainCharacter.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Journal/ActionWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/CharacterController.h"
#include "Components/AudioComponent.h"
#include "Components/VerticalBox.h"
#include "Journal/CheckpointListEntry.h"
#include "Journal/CheckpointMapButton.h"
#include "Journal/JournalManager.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"
#include "SaveSystem/UniqueIDComponent.h"
#include "Journal/OverlayBlockZoom.h"
#include "Components/ScrollBox.h"
#include "Journal/IconTableRow.h"
#include "Journal/IndexSpacer.h"
#include "Journal/MapIconsLegend.h"

UCheckpointListEntry* UMapWidget::FindEntryByCheckpoint(const TArray<UCheckpointListEntry*>& CheckpointList,
                                                        ACheckpoint* Checkpoint)
{
	for (UCheckpointListEntry* Entry : CheckpointList)
	{
		if (Entry && Entry -> BindedCheckpoint == Checkpoint)
		{
			return Entry;
		}
	}
	return nullptr;
}

void UMapWidget::BroadcastTeleportRequested(FVector TeleportLocation)
{
	OnTeleportRequest.Broadcast(TeleportLocation);
}


void UMapWidget::CreateCollectableIcon(ACollectableItems* CollectableItem)
{
	if (!CollectableItem->IsTrigger)
	{
		UImage* Image = NewObject<UImage>(this);
		USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
		SaveGame = SaveSubsystem->LoadGame();
		if (SaveGame -> DiscoveredCollectibles.Contains(CollectableItem))
		{
			Image->SetBrushFromTexture(Manager->GetIconFromDataTable(CollectableItem->MapIconType, false));
		}
		else if (CollectableItem->IsHidden())
		{
			Image->SetBrushFromTexture(Manager->GetIconFromDataTable(CollectableItem->MapIconType, true));
		}
		else
			return;
		FVector2D MapSize = MapIcon->GetCachedGeometry().GetLocalSize();
		if (MapSize.IsNearlyZero())
		{
			MapSize = FVector2D(1200.f, 1200.f);
		}
		FVector2D PixelPosition = Manager->ConvertWorldToMapLocation(CollectableItem->GetActorLocation(), Manager->MapBounds, MapSize);
		FVector2D Center = MapSize*0.5f;
		float OffsetFromCenterX = PixelPosition.X - Center.X;
		FVector2D FinalPosition = FVector2D(PixelPosition.X - (OffsetFromCenterX * 0.3f), PixelPosition.Y);

		Image ->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
		if (CollectableItem->MapIconType != EMapIconType::MapFragment)
		{
			UCanvasPanelSlot* MySlot = IconsCanvas->AddChildToCanvas(Image);
			if (MySlot)
			{
				MySlot->SetAnchors(FAnchors(0.0f, 0.0f));
				MySlot->SetPosition(FinalPosition);
				MySlot -> SetAlignment(FVector2D(0.5f, 0.5f));
				MySlot->SetAutoSize(true);
				//MySlot->SetSize(FVector2D(MapCreatedIconSize, MapCreatedIconSize));
				Image->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			UCanvasPanelSlot* MySlot = AlwaysVisibleCanvas->AddChildToCanvas(Image);
			if (MySlot)
			{
				MySlot->SetAnchors(FAnchors(0.0f, 0.0f));
				MySlot->SetPosition(FinalPosition);
				MySlot -> SetAlignment(FVector2D(0.5f, 0.5f));
				MySlot->SetAutoSize(true);
				//MySlot->SetSize(FVector2D(MapCreatedIconSize, MapCreatedIconSize));
				Image->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

UCheckpointMapButton* UMapWidget::CreateCheckpointButton(ACheckpoint* Checkpoint)
{
	FVector2D MapSize = MapIcon->GetCachedGeometry().GetLocalSize();
	if (MapSize.IsNearlyZero())
	{
		MapSize = FVector2D(1200.f, 1200.f);
	}
	UCheckpointMapButton* CheckpointWidget = CreateWidget<UCheckpointMapButton>(this, CheckpointButtonClass);
	CheckpointWidget ->SetCheckpoint(Checkpoint);
	CheckpointWidget->SetIconTexture(Manager->GetIconFromDataTable(Checkpoint->IconType, false));
	CheckpointWidget -> SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	CheckpointWidget->OnTeleportRequested.AddDynamic(this, &ThisClass::BroadcastTeleportRequested);
	UCanvasPanelSlot* MySlot = AlwaysVisibleCanvas->AddChildToCanvas(CheckpointWidget);
	FVector2D PixelPosition = (Manager->ConvertWorldToMapLocation(Checkpoint->GetActorLocation(), Manager->MapBounds, MapSize));
	FVector2D Center = MapSize*0.5f;
	float OffsetFromCenterX = PixelPosition.X - Center.X;
	FVector2D FinalPosition = FVector2D(PixelPosition.X - (OffsetFromCenterX * 0.3f), PixelPosition.Y);
	
	if (MySlot)
	{
		MySlot->SetAnchors(FAnchors(0.0f, 0.0f));
		MySlot -> SetAlignment(FVector2D(0.5f, 0.5f));
		MySlot->SetPosition(FinalPosition);
		MySlot->SetAutoSize(true);
		//MySlot->SetSize(FVector2D(MapCreatedIconSize, MapCreatedIconSize));
		CheckpointWidget->SetVisibility(ESlateVisibility::Visible);
	}

	return CheckpointWidget;
}

void UMapWidget::CreatePlayerIcon()
{
	AMainCharacter* MC = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MC)
	{
		UImage* Image = NewObject<UImage>(this);
		Image->SetBrushFromTexture(Manager->GetIconFromDataTable(EMapIconType::MainCharacter, false));
		FVector2D MapSize = MapIcon->GetCachedGeometry().GetLocalSize();
		if (MapSize.IsNearlyZero())
		{
			MapSize = FVector2D(1200.f, 1200.f);
		}
		FVector2D PixelPosition = Manager->ConvertWorldToMapLocation(MC->GetActorLocation(), Manager->MapBounds, MapSize);
		Image ->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
		FVector2D Center = MapSize*0.5f;
		float OffsetFromCenterX = PixelPosition.X - Center.X;
		FVector2D FinalPosition = FVector2D(PixelPosition.X - (OffsetFromCenterX * 0.3f), PixelPosition.Y);
		UCanvasPanelSlot* MySlot = AlwaysVisibleCanvas->AddChildToCanvas(Image);
		if (MySlot)
		{
			MySlot->SetAnchors(FAnchors(0.0f, 0.0f));
			MySlot->SetPosition(FinalPosition);
			MySlot -> SetAlignment(FVector2D(0.5f, 0.5f));
			MySlot->SetAutoSize(true);
			//MySlot->SetSize(FVector2D(MapCreatedIconSize, MapCreatedIconSize));
			Image->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMapWidget::NativeOnActivated()
{
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	SaveGame = SaveSubsystem->LoadGame();
	BlockZoom -> MapWidget = this;
	LastFocusedEntry = nullptr;
	ZoomLevel = 1.0f;
	FocusIndex = 0;
	MapButtons.Empty();
	Super::NativeOnActivated();
	IndexPanelSize = CheckpointVerticalBox->GetCachedGeometry().GetLocalSize();
}

void UMapWidget::TryLerp()
{
	if (SaveGame->MapFragments.Contains(1))
	{
		if (SaveGame -> MapFragmentsUnlocked.Contains(1))
		{
			Blocker1 -> GetDynamicMaterial() -> SetScalarParameterValue(FName("AlreadyFaded"), 0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Lerping"));
			LerpMaterial(Blocker1);
			SaveGame -> AddMapFragmentsUnlocked(1);
		}
	}
}

void UMapWidget::PopulateLegend()
{
	if (!IconsDataTable || !MapIconLegend || !LegendVerticalBox) return;

	static const FString ContextString(TEXT("Item Context"));
	TArray<FIconTableRow*> AllRows;
	IconsDataTable->GetAllRows(ContextString, AllRows);

	TSet<FString> AddedNames;
	
	for (FIconTableRow* Row : AllRows)
	{
		if (!Row)
		{
			continue;
		}
		FString Name = Row ->IconName.ToString();
		if (AddedNames.Contains(Name))
		{
			continue;
		}
		AddedNames.Add(Name);
		UMapIconsLegend* NewWidget = CreateWidget<UMapIconsLegend>(this, MapIconLegend);
		if (NewWidget)
		{
			NewWidget -> Icon ->SetBrushFromTexture(Row->EntryImage);
			NewWidget ->IconName->SetText(FText::FromName(Row->IconName));
			LegendVerticalBox->AddChildToVerticalBox(NewWidget);
			UIndexSpacer* Space = CreateWidget<UIndexSpacer>(this, Spacer);
			LegendVerticalBox->AddChildToVerticalBox(Space);
		}
	}
}

void UMapWidget::PlayFadeSound()
{
	if (FocusAudioComponent && FocusAudioComponent->IsPlaying())
	{
		FocusAudioComponent->Stop();
		FocusAudioComponent->DestroyComponent();
		FocusAudioComponent = nullptr;
	}
	FocusAudioComponent = UGameplayStatics::SpawnSound2D(
		GetWorld(),
		FadeSound,
		1.0f,
		1.0f,
		0.0f,
		nullptr,
		false
	);
	
	/*if (FocusAudioComponent)
	{
		FocusAudioComponent->Play();
	}*/
}

void UMapWidget::StopFadeSound()
{
	if (FocusAudioComponent != nullptr)
	{
		FocusAudioComponent->Stop();
		FocusAudioComponent->DestroyComponent();
		FocusAudioComponent = nullptr;
	}
}

void UMapWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MapOffset = FVector2D::ZeroVector;
	MapScaleBox->SetRenderScale(FVector2D(1.0f, 1.0f));
	MapContainer->SetRenderTranslation(FVector2D::ZeroVector);

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
}

void UMapWidget::CenterMapOnPixel(UCheckpointListEntry* Entry)
{
	LastFocusedEntry = Entry;
	if (Entry != nullptr)
	{
		UCheckpointMapButton* Bt = IndexButtonToMapButton[Entry];
		Bt -> ActivateBorderImage(true);
		if (ZoomLevel > 1.0f)
		{
			
			FVector2D UnscaledMapSize = MapIcon->GetCachedGeometry().GetLocalSize();
			FVector2D ScaledMapSize = UnscaledMapSize * ZoomLevel;
			FVector2D Pixel = Manager->ConvertWorldToMapLocation(Bt->BoundCheckpoint->GetActorLocation(), Manager->MapBounds, ScaledMapSize);
			
			FVector2D ViewSize = MapScaleBox->GetCachedGeometry().GetLocalSize();

			FVector2D DesiredOffset = (ViewSize * 0.5f) - Pixel;
			
			DesiredOffset.X = FMath::Clamp(DesiredOffset.X, ViewSize.X - ScaledMapSize.X, 0.f);
			DesiredOffset.Y = FMath::Clamp(DesiredOffset.Y, ViewSize.Y - ScaledMapSize.Y, 0.f);

			MapOffset = DesiredOffset;
			MapContainer->SetRenderTranslation(MapOffset);
		}
	}
	else return;
}

void UMapWidget::PopulateCheckpointList()
{
	if (!CheckpointVerticalBox || !CheckpointListEntry) return;

	CheckpointVerticalBox->ClearChildren();

	for (ACheckpoint* CP : Manager->Checkpoints)
	{
		UCheckpointListEntry* Entry = CreateWidget<UCheckpointListEntry>(this, CheckpointListEntry);
		Entry->SetIsFocusable(false);
		Entry->SetIsSelectable(false);
		Entry -> MapWidget = this;
		Entry->BindedCheckpoint = nullptr;
		Entry->CheckpointName->SetText(FText::FromString("Undiscovered"));

		CheckpointVerticalBox->AddChild(Entry);
		
		if (Spacer)
		{
			UIndexSpacer* Widget = CreateWidget<UIndexSpacer>(this, Spacer);
			CheckpointVerticalBox->AddChild(Widget);
		}
	}
}

void UMapWidget::UpdateButtons()
{
	MapButtons.Empty();
    IndexButtonToMapButton.Empty();
	
    TArray<UCheckpointListEntry*> Entries;
    int32 ChildCount = CheckpointVerticalBox->GetChildrenCount();
    for (int32 i = 0; i < ChildCount; i++)
    {
        UWidget* Child = CheckpointVerticalBox->GetChildAt(i);
        if (UCheckpointListEntry* Entry = Cast<UCheckpointListEntry>(Child))
        {
            Entries.Add(Entry);
        }
    }
	
    const int32 Num = FMath::Min(Entries.Num(), Manager->Checkpoints.Num());
    for (int32 i = 0; i < Num; i++)
    {
        ACheckpoint* CP = Manager->Checkpoints[i];
        UCheckpointListEntry* Entry = Entries[i];

        if (Manager->AvailableCheckpoints.Contains(CP))
        {
            Entry->BindedCheckpoint = CP;
            Entry->CheckpointName->SetText(CP->CheckpointName);
        	Entry -> MapWidget = this;
            UCheckpointMapButton* MapBtn = CreateCheckpointButton(CP);
            IndexButtonToMapButton.Add(Entry, MapBtn);
        	
            Entry->OnCheckpointEntryFocused.RemoveDynamic(this, &UMapWidget::CenterMapOnPixel);
            Entry->OnCheckpointEntryClicked.RemoveDynamic(this, &UMapWidget::ClickMapButton);
            Entry->OnCheckpointEntryUnFocused.RemoveDynamic(this, &UMapWidget::DisableButtonBorderImage);
        	

            Entry->OnCheckpointEntryFocused.AddDynamic(this, &UMapWidget::CenterMapOnPixel);
            Entry->OnCheckpointEntryClicked.AddDynamic(this, &UMapWidget::ClickMapButton);
            Entry->OnCheckpointEntryUnFocused.AddDynamic(this, &UMapWidget::DisableButtonBorderImage);

        	Entry -> SetIsEnabled(true);
            Entry->SetIsFocusable(true);
            Entry->SetIsSelectable(true);
        	
            MapButtons.Add(Entry);
            Entry->IndexValue = MapButtons.Find(Entry);
        }
        else
        {
            // undiscovered
            Entry->BindedCheckpoint = nullptr;
            Entry->CheckpointName->SetText(FText::FromString("Undiscovered"));
            Entry->SetIsFocusable(false);
            Entry->SetIsSelectable(false);
        	Entry -> SetIsEnabled(false);
        }
    }
}

ACheckpoint* UMapWidget::FocusNearestCheckpoint()
{
	float MinDistance = FLT_MAX;
	UCheckpointListEntry* EntryToFocus = nullptr;
	for (ACheckpoint* CP : Manager->AvailableCheckpoints)
	{
		float CurrentDistance = FVector::DistSquared(GetWorld()->GetFirstPlayerController()->GetPawn() ->GetActorLocation(), CP -> GetActorLocation());
		if (CurrentDistance < MinDistance)
		{
			MinDistance = CurrentDistance;
			EntryToFocus = FindEntryByCheckpoint(MapButtons, CP);
		}
	}
	if (EntryToFocus && EntryToFocus -> IsFocusable()) 
	{
		EntryToFocus -> SetFocus();
		EntryToFocus -> SetIsSelected(true);
		EntryToFocus -> HandleFocused();
		return EntryToFocus -> BindedCheckpoint;
	}
	else
	{
		return nullptr;
	}
}

void UMapWidget::LerpMaterial(UImage* Blocker)
{
	if (!Blocker)
	{
		UE_LOG(LogTemp, Warning, TEXT("!Blocker"));
		return;
	}
	PlayFadeSound();
	UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(Blocker->GetDynamicMaterial());
	DynMat -> SetScalarParameterValue(FName("AlreadyFaded"), 1);
	DynMat->SetScalarParameterValue(FName("CanFade"), 1);
	DynMat->SetScalarParameterValue(FName("CurrentTime"), FApp::GetCurrentTime() - GStartTime);
}

void UMapWidget::SetFocusCheckpointEntry()
{
	if (CheckpointVerticalBox)
	{
		int32 ChildCount = CheckpointVerticalBox->GetChildrenCount();
		for (int32 i = 0; i < ChildCount; i++)
		{
			UWidget* Child = CheckpointVerticalBox->GetChildAt(i);
			UCheckpointListEntry* Entry = Cast<UCheckpointListEntry>(Child);
			if (Entry && Entry->IsFocusable())
			{
				Entry->SetFocus();
				Entry->SetIsSelected(true);
				//CHANGE NECESSARY
				Entry -> HandleFocused();
				break;
			}
		}
	}
}

void UMapWidget::ResetEntryClicked()
{
	EntryClicked = nullptr;
}

/*
void UMapWidget::PanMap(const FInputActionValue& Value)
{
	if ((CanPan && ZoomLevel >1.0f) || (static_cast<bool>(EHardwareDevicePrimaryType::Gamepad) && ZoomLevel > 1.0f))
	{
		FVector2D Delta = Value.Get<FVector2D>();
		MapOffset += Delta * (5.0f / ZoomLevel);

		// Clamp against map edges
		FVector2D MapSize = MapIcon->GetBrush().ImageSize * ZoomLevel;
		FVector2D ViewSize = MapIcon->GetCachedGeometry().GetLocalSize();
		MapOffset.X = FMath::Clamp(MapOffset.X, -(MapSize.X - ViewSize.X), IndexPanelSize.X);
		MapOffset.Y = FMath::Clamp(MapOffset.Y, -(MapSize.Y - ViewSize.Y), 0.f);

		MapContainer->SetRenderTranslation(MapOffset);
	}
}
*/
void UMapWidget::ZoomMap(const FInputActionValue& Value, UCheckpointListEntry* Entry)
{
	float Delta = Value.Get<float>();
	float PrevZoom = ZoomLevel;
	ZoomLevel = FMath::Clamp(ZoomLevel + Delta * 0.1f, 1.0f, 1.5f);
	MapScaleBox->SetRenderScale(FVector2D(ZoomLevel, ZoomLevel));
	
	FVector2D ViewSize = MapIcon->GetCachedGeometry().GetLocalSize();
	FVector2D MapSizeAfter = MapIcon->GetBrush().ImageSize * ZoomLevel;
	
	FVector2D ViewCenter = ViewSize * 0.5f;
	FVector2D MapCenterBefore = (ViewCenter - MapOffset) / PrevZoom;
	
	MapOffset = ViewCenter - (MapCenterBefore * ZoomLevel);
	
	MapOffset.X = FMath::Clamp(MapOffset.X, -(MapSizeAfter.X - ViewSize.X), 0.f);
	MapOffset.Y = FMath::Clamp(MapOffset.Y, -(MapSizeAfter.Y - ViewSize.Y), 0.f);

	if (FMath::IsNearlyEqual(ZoomLevel, 1.0f))
	{
		MapOffset = FVector2D::ZeroVector;
		MapContainer->SetRenderTranslation(MapOffset);
	}
	else
		CenterMapOnPixel(Entry);

}

void UMapWidget::SetCanPan(const FInputActionValue& Value)
{
	CanPan = Value.Get<bool>();
}

void UMapWidget::ClickMapButton(UCheckpointListEntry* Entry)
{
	UCheckpointMapButton* Bt = IndexButtonToMapButton[Entry];
	ACheckpoint* CP = FocusNearestCheckpoint();
	if (CP)
		Bt->MapButtonClick(CP);
	EntryClicked = Entry;
}

void UMapWidget::DisableButtonBorderImage(UCheckpointListEntry* Entry)
{
	if (UCheckpointMapButton** BtPtr = IndexButtonToMapButton.Find(Entry))
	{
		UCheckpointMapButton* Bt = *BtPtr;
		if (Bt)
		{
			Bt->ActivateBorderImage(false);
		}
	}
}

FReply UMapWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey == EKeys::W || PressedKey == EKeys::S)
	{
		int32 NumButtons = MapButtons.Num();
		if (NumButtons == 0)
		{
			return FReply::Unhandled();
		}

		int32 Direction = (PressedKey == EKeys::W) ? -1 : 1;
		FocusIndex = (FocusIndex + Direction + NumButtons) % NumButtons;
		MapButtons[FocusIndex] -> SetFocus();
		MapButtons[FocusIndex] -> SetIsSelected(true);

		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}

FReply UMapWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == UpAction -> KeyboardKey || Key == UpAction ->GamepadKey|| Key == EKeys::Up)
	{
		UpAction -> PlayActionAnimation();
		return FReply::Unhandled();
	}
	if (Key == DownAction -> KeyboardKey || Key == DownAction ->GamepadKey|| Key == EKeys::Down)
	{
		DownAction -> PlayActionAnimation();
		return FReply::Unhandled();
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UMapWidget::SetFocusIndex(int32 Index)
{
	FocusIndex = Index;
}

