#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "NewBaseWidgetClass.h"
#include "CommonRichTextBlock.h"
#include "BestiaryPageWidget.generated.h"

UCLASS()
class TBO_01_API UBestiaryPageWidget : public UNewBaseWidgetClass
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonRichTextBlock* EntryTitleText;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* EntryImage;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* TemperIcon;
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonRichTextBlock* ProtagonistNoteLeft;
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonRichTextBlock* ProtagonistNoteRight;
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonRichTextBlock* Hint1;
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonRichTextBlock* Hint2;
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonRichTextBlock* Hint3;
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonRichTextBlock* Hint4;
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonRichTextBlock* Lore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* Blocker1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* Blocker2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* Blocker3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* Blocker4;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Scribble1;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Scribble2;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Scribble3;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Scribble4;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Scribble5;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Scribble6;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Scribble7;
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Scribble8;
	UPROPERTY(meta = (BindWidgetOptional))
	class UBorder* BorderNoteLeft;
	UPROPERTY(meta = (BindWidgetOptional))
	class UBorder* BorderNoteRight;
	UPROPERTY(meta = (BindWidgetOptional))
	class UBorder* BorderTitle;
	UPROPERTY(meta = (BindWidgetOptional))
	class UBorder* BorderLore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CreatureSelected = FName("Creature");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaterialFadeTime;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Blockers")
	TArray<UImage*> BlockersFading;
	virtual bool Initialize() override;
	UFUNCTION(BlueprintCallable)
	void LerpMaterial(UImage* Blocker);
	void ResetBlockers(UImage* Blocker);
	virtual void NativeOnActivated() override;
	void CompleteAllFades();
	UFUNCTION(BlueprintCallable)
	void ResetCreatureSelected();

	UFUNCTION(BlueprintCallable)
	void InitializeScribbles();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* FadeSound;
	UPROPERTY(Transient)
	UAudioComponent* FocusAudioComponent = nullptr;
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI|Blockers")
	TMap<UImage*, UMaterialInterface*> BlockerMaterials;
};
