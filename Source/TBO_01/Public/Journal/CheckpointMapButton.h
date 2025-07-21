#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CheckpointMapButton.generated.h"

class ACheckpoint;
class UConfirmationModalWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeleportRequested, FVector, TeleportLocation);

UCLASS()
class TBO_01_API UCheckpointMapButton : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;
	UPROPERTY(meta = (BindWidget))
	class UImage* BorderImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ACheckpoint* BoundCheckpoint;
	
	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	void SetIconTexture(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category="Checkpoint")
	void SetCheckpoint(ACheckpoint* InCheckpoint);
	
	UFUNCTION(BlueprintImplementableEvent)
	void TeleportPlayer();

	UFUNCTION(BlueprintCallable)
	void ActualTeleport();
	
	UFUNCTION(BlueprintImplementableEvent)
	void MapButtonClick(ACheckpoint* Checkpoint);

	UFUNCTION(BlueprintCallable)
	void ActivateBorderImage(bool Visible);

	UPROPERTY(BlueprintAssignable)
	FOnTeleportRequested OnTeleportRequested;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Checkpoint")
	TSubclassOf<UConfirmationModalWidget> ConfirmationModalClass;

	UFUNCTION(BlueprintCallable)
	void BroadcastOnTeleportRequested();
};
