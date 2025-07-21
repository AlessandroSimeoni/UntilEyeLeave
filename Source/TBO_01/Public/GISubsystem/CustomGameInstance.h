#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GISubsystem/SceneHandlerGISubsystem.h"
#include "Engine/DataTable.h"
#include "CustomGameInstance.generated.h"


class UCommonActivatableWidget;
class UUniqueIDComponent;

UCLASS()
class TBO_01_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	
	virtual void Init() override;
	UFUNCTION(BlueprintCallable, Category = "CustomGameInstance")
	USceneHandlerGISubsystem* GetSceneHandlerSubsystem() const;
    
private:
	UPROPERTY()
	USceneHandlerGISubsystem* MySubsystem;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Journal")
	UDataTable* BestiaryDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundMix> SoundMix = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundClass> SFXSoundClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skip Cutscene Widget Class")
	TSubclassOf<UCommonActivatableWidget> SkipCutsceneWidgetClass = nullptr;
};
