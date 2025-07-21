#pragma once

#include "CoreMinimal.h"
#include "Journal/NewBaseWidgetClass.h"
#include "Journal/CollectableItems.h"
#include "WorldActors/Checkpoint.h"
#include "InputActionValue.h"
#include "MapWidget.generated.h"

class UMapIconsLegend;
class UIndexSpacer;
class UInputAction;
class UCanvasPanel;
class UCheckpointMapButton;
class UCheckpointListEntry;
class UImage;
class UOverlay;
class UScaleBox;
class UVerticalBox;
class UScrollBox;
class UOverlayBlockZoom;
class UActionWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeleportRequest, FVector, TeleportLocation);

UCLASS()
class TBO_01_API UMapWidget : public UNewBaseWidgetClass
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScaleBox* MapScaleBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* MapContainer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* MapIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* IconsCanvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* FogOverlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* AlwaysVisibleCanvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* CheckpointVerticalBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* EntryScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UImage* Blocker1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlayBlockZoom* BlockZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UVerticalBox* LegendVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "Actions")
	UActionWidget* UpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "Actions")
	UActionWidget* DownAction;

	UPROPERTY(EditDefaultsOnly, Category= "Map")
	TSubclassOf<UCheckpointMapButton> CheckpointButtonClass;
	UPROPERTY(EditDefaultsOnly, Category= "Map")
	TSubclassOf<UCheckpointListEntry> CheckpointListEntry;
	UPROPERTY(EditDefaultsOnly, Category= "Map")
	TSubclassOf<UMapIconsLegend> MapIconLegend;
	UPROPERTY(EditDefaultsOnly, Category= "Map")
	TSubclassOf<UIndexSpacer> Spacer;
	UPROPERTY()
	TMap<UCheckpointListEntry*, UCheckpointMapButton*> IndexButtonToMapButton;
	UPROPERTY()
	UCheckpointListEntry* LastFocusedEntry = nullptr;
	UPROPERTY(BlueprintAssignable)
	FOnTeleportRequest OnTeleportRequest;

	UPROPERTY(BlueprintReadOnly)
	int32 FocusIndex = 0;
private:
	UPROPERTY()
	float ZoomLevel = 1.0f;
	UPROPERTY()
	FVector2D MapOffset = FVector2D::ZeroVector;
	UPROPERTY()
	bool CanPan = false;
	UPROPERTY()
	FVector2D IndexPanelSize;
	UPROPERTY()
	TArray<UCheckpointListEntry*> MapButtons = TArray<UCheckpointListEntry*>();
	UCheckpointListEntry* FindEntryByCheckpoint(const TArray<UCheckpointListEntry*>& CheckpointList, ACheckpoint* Checkpoint);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Blockers")
	TMap<UImage*, UMaterialInterface*> BlockerMaterials;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* IconsDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class AJournalManager* Manager;
	UPROPERTY()
	class UTBOSaveGame* SaveGame;
	UPROPERTY(EditDefaultsOnly)
	float VerticalPanThreshold;
	UPROPERTY(EditDefaultsOnly)
	float HorizontalPanThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCheckpointListEntry* EntryClicked = nullptr;
	UFUNCTION()
	void BroadcastTeleportRequested(FVector TeleportLocation);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* FadeSound;
	UPROPERTY(Transient)
	UAudioComponent* FocusAudioComponent = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	void CreateCollectableIcon(ACollectableItems* CollectableItem);
	UFUNCTION(BlueprintCallable)
	UCheckpointMapButton* CreateCheckpointButton(ACheckpoint* Checkpoint);
	UFUNCTION(BlueprintCallable)
	void CreatePlayerIcon();
	UFUNCTION(BlueprintCallable)
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	UFUNCTION()
	void CenterMapOnPixel(UCheckpointListEntry* Entry);
	UFUNCTION(BlueprintCallable)
	void PopulateCheckpointList();
	//void PanMap(const FInputActionValue& Value);
	void ZoomMap(const FInputActionValue& Value, UCheckpointListEntry* Entry);
	void SetCanPan(const FInputActionValue& Value);
	UFUNCTION()
	void ClickMapButton(UCheckpointListEntry* Entry);
	UFUNCTION()
	void DisableButtonBorderImage(UCheckpointListEntry* Entry);
	UFUNCTION(BlueprintCallable)
	void SetFocusCheckpointEntry();
	UFUNCTION(BlueprintCallable)
	void ResetEntryClicked();
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	UFUNCTION(BlueprintCallable)
	void SetFocusIndex(int32 Index);
	UFUNCTION(BlueprintCallable)
	void UpdateButtons();
	UFUNCTION(BlueprintCallable)
	ACheckpoint* FocusNearestCheckpoint();
	void LerpMaterial(UImage* Blocker);
	UFUNCTION(BlueprintCallable)
	void TryLerp();
	UFUNCTION(BlueprintCallable)
	void PopulateLegend();
	UFUNCTION(BlueprintCallable)
	void PlayFadeSound();
	UFUNCTION(BlueprintCallable)
	void StopFadeSound();
};

