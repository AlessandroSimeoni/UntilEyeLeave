#include "Journal/EntryJournalButton.h"
#include "GISubsystem/CustomGameInstance.h"
#include "Components/Image.h"
#include "CommonTextBlock.h"
#include "Components/AudioComponent.h"
#include "Components/Border.h"
#include "Journal/JournalEntry.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"


void UEntryJournalButton::UpdateJournalPage(UBestiaryPageWidget* Page)
{
	if (Page -> CreatureSelected != EntryRow.RowName)
	{
		Page -> CreatureSelected = EntryRow.RowName;
		StopFadeSound(Page);
		PlaySound = false;
		Page -> InitializeScribbles();
		Page -> CompleteAllFades();
		Page -> ResetBlockers(Page -> Blocker1);
		Page -> ResetBlockers(Page -> Blocker2);
		Page -> ResetBlockers(Page -> Blocker3);
		Page -> ResetBlockers(Page -> Blocker4);
		
		USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
		UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
		FJournalEntry* CurrentEntry = SaveGame -> PersistentJournalEntries.Find(EntryRow.RowName);
	
		if (Page -> EntryTitleText)
		{
			Page -> EntryTitleText -> SetText(Row.EntryTitle);
		}
		if (Page -> EntryImage)
		{
			Page -> EntryImage -> SetBrushFromTexture(Row.EntryImage);
		}
		if (Page -> TemperIcon)
		{
			Page -> TemperIcon ->SetBrushFromTexture(Row.TemperIcon);
		}
		if (Page -> ProtagonistNoteLeft)
		{
			Page -> ProtagonistNoteLeft -> SetText(Row.ProtagonistNoteLeftSide);
		}
		if (Page -> ProtagonistNoteRight)
		{
			Page -> ProtagonistNoteRight -> SetText(Row.ProtagonistNoteRightSide);
		}
		if (Page -> Hint1)
		{
			Page -> Hint1 -> SetText(Row.Hint1);
		}
		if (Page -> Hint2)
		{
			Page -> Hint2 -> SetText(Row.Hint2);
		}
		if (Page -> Hint3)
		{
			Page -> Hint3 -> SetText(Row.Hint3);
		}
		if (Page -> Hint4)
		{
			Page -> Hint4 -> SetText(Row.Hint4);
		}
		if (Page -> Lore)
		{
			Page -> Lore -> SetText(Row.Lore);
		}
	
		if (CurrentEntry)
		{
			if (!SaveGame -> EntriesFragmentUnlocked.Contains(CurrentEntry -> EntryID))
				SaveGame -> CreateEntriesFragmentsUnlocked(CurrentEntry-> EntryID);
			FIntArray* IntArray = SaveGame -> EntriesFragmentUnlocked.Find(CurrentEntry -> EntryID);
			if (CurrentEntry->FragmentsCollected.Contains(1))
			{
				if (!IntArray->IntArrayCollected.Contains(1))
				{
					Page -> LerpMaterial(Page -> Blocker1);
					PlaySound = true;
					SaveGame -> UpdateEntriesFragmentsUnlocked(CurrentEntry-> EntryID, 1);
				}
				else
				{
					Page -> Blocker1 -> GetDynamicMaterial() -> SetScalarParameterValue(FName("AlreadyFaded"), 0);
				}
				if (!Page -> Hint1->GetText().IsEmpty() && Page -> Scribble1 != nullptr && Page -> Scribble2 != nullptr)
				{
					//Page -> Scribble1 -> SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Page -> Scribble1 -> SetRenderOpacity(1);
					Page -> Scribble2 -> SetRenderOpacity(1);
				}
				if (!Page -> Hint2->GetText().IsEmpty() && Page -> Scribble3 != nullptr && Page -> Scribble4 != nullptr)
				{
					//Page -> Scribble2 -> SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Page -> Scribble3 -> SetRenderOpacity(1);
					Page -> Scribble4 -> SetRenderOpacity(1);
				}
			}
			if (CurrentEntry->FragmentsCollected.Contains(2))
			{
				if (!IntArray->IntArrayCollected.Contains(2))
				{
					Page -> LerpMaterial(Page -> Blocker2);
					PlaySound = true;
					SaveGame -> UpdateEntriesFragmentsUnlocked(CurrentEntry-> EntryID, 2);
				}
				else
				{
					Page -> Blocker2 -> GetDynamicMaterial() -> SetScalarParameterValue(FName("AlreadyFaded"), 0);
				}
				if (!Page->EntryTitleText->GetText().IsEmpty() && Page->BorderTitle != nullptr)
				{
					Page-> BorderTitle ->SetRenderOpacity(1);
				}
				/*if (Page->EntryImage != nullptr)
				{
					Page-> EntryImage -> SetRenderOpacity(1);
				}*/
				if (Page -> TemperIcon != nullptr)
				{
					Page-> TemperIcon ->SetRenderOpacity(1);
				}
			}
			if (CurrentEntry->FragmentsCollected.Contains(3))
			{
				if (!IntArray->IntArrayCollected.Contains(3))
				{
					Page -> LerpMaterial(Page -> Blocker3);
					PlaySound = true;
					SaveGame -> UpdateEntriesFragmentsUnlocked(CurrentEntry-> EntryID, 3);
				}
				else
				{
					Page -> Blocker3 -> GetDynamicMaterial() -> SetScalarParameterValue(FName("AlreadyFaded"), 0);
				}
				if (!Page -> Hint3->GetText().IsEmpty() && Page -> Scribble5 != nullptr && Page -> Scribble6 != nullptr)
				{
					//Page -> Scribble3 -> SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Page -> Scribble5 -> SetRenderOpacity(1);
					Page -> Scribble6 -> SetRenderOpacity(1);
				}
				if (!Page -> Hint4->GetText().IsEmpty() && Page -> Scribble7 != nullptr && Page -> Scribble8 != nullptr)
				{
					//Page -> Scribble4 -> SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Page -> Scribble7 -> SetRenderOpacity(1);
					Page -> Scribble8 -> SetRenderOpacity(1);
				}
			}
			if (CurrentEntry->FragmentsCollected.Contains(4))
			{
				if (!IntArray->IntArrayCollected.Contains(4))
				{
					Page -> LerpMaterial(Page -> Blocker4);
					PlaySound = true;
					SaveGame -> UpdateEntriesFragmentsUnlocked(CurrentEntry-> EntryID, 4);
				}
				else
				{
					Page -> Blocker4 -> GetDynamicMaterial() -> SetScalarParameterValue(FName("AlreadyFaded"), 0);
				}
				if (!Page ->ProtagonistNoteLeft ->GetText().IsEmpty() &&  Page -> BorderNoteLeft != nullptr)
				{
					Page->BorderNoteLeft->SetRenderOpacity(1);
				}
				if (!Page->ProtagonistNoteRight ->GetText().IsEmpty() && Page -> BorderNoteRight != nullptr)
				{
					Page -> BorderNoteRight -> SetRenderOpacity(1);
				}
				if (!Page->Lore->GetText().IsEmpty() && Page->BorderLore != nullptr)
				{
					Page-> BorderLore ->SetRenderOpacity(1);
				}
			}
		}
		if (PlaySound && (Page -> FadeSound != nullptr))
		{
			UE_LOG(LogTemp, Warning, TEXT("FadeSound"));
			PlayFadeSound(Page);
		}
	}
}

UEntryJournalButton* UEntryJournalButton::UpdateButtonInfo()
{
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
	if (SaveGame->PersistentJournalEntries.Contains(EntryRow.RowName))
	{
		EntryTitleText -> SetText(Row.EntryTitle);
		EntryIcon -> SetBrushFromTexture(Row.EntryIcon);
		FragmentDiscovered->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FragmentDiscovered -> SetOpacity(1);
		SetIsEnabled(true);
		SetIsSelectable(true);
		SetIsFocusable(true);
	}
	return this;
}

void UEntryJournalButton::InitializeButton(FBestiaryTableRow InRow)
{
	Row = InRow;
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
	if (SaveGame->PersistentJournalEntries.Contains(EntryRow.RowName))
	{
		EntryTitleText -> SetText(Row.EntryTitle);
		EntryIcon -> SetBrushFromTexture(Row.EntryIcon);
		//FragmentDiscovered -> SetVisibility(ESlateVisibility::Collapsed);
		FragmentDiscovered -> SetOpacity(0);
		SetIsEnabled(true);
		SetIsSelectable(true);
		SetIsFocusable(true);
	}
	else
	{
		EntryTitleText -> SetText(FText::FromString("Undiscovered"));
		EntryIcon -> SetBrushFromTexture(QuestionMarks);
		//FragmentDiscovered -> SetVisibility(ESlateVisibility::Collapsed);
		FragmentDiscovered -> SetOpacity(0);
		SetIsSelectable(false);
		SetIsFocusable(false);
		SetIsEnabled(false);
	}
}

void UEntryJournalButton::UpdateDiscoveryIcon()
{
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* SaveGame = SaveSubsystem->LoadGame();
	FJournalEntry* Entry = SaveGame->PersistentJournalEntries.Find(EntryRow.RowName);
	FIntArray* Array = SaveGame -> EntriesFragmentUnlocked.Find(EntryRow.RowName);
	int32 EntryNum;
	int32 ArrayNum;
	if (Entry != nullptr)
	{
		EntryNum = Entry->FragmentsCollected.Num();
	}
	else
	{
		EntryNum = 0;
	}
	if (Array != nullptr)
	{
		ArrayNum = Array -> IntArrayCollected.Num();
	}
	else
	{
		ArrayNum = 0;
	}
	if (EntryNum != ArrayNum)
	{
		UE_LOG(LogTemp, Warning, TEXT("Activated icon"));
		FragmentDiscovered->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FragmentDiscovered -> SetOpacity(1);
	}
}

void UEntryJournalButton::PlayFadeSound(UBestiaryPageWidget* Page)
{
	if (Page->FocusAudioComponent && Page->FocusAudioComponent->IsPlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("STILL PLAYING"));
		Page->FocusAudioComponent->Stop();
		Page->FocusAudioComponent->DestroyComponent();
		Page->FocusAudioComponent = nullptr;
	}
	Page -> FocusAudioComponent = UGameplayStatics::SpawnSound2D(
		GetWorld(),
		Page -> FadeSound,
		1.0f,
		1.0f,
		0.0f,
		nullptr,
		false
	);
	
	/*if (Page->FocusAudioComponent)
	{
		Page->FocusAudioComponent->Play();
	}*/
}

void UEntryJournalButton::StopFadeSound(UBestiaryPageWidget* Page)
{
	if (Page -> FocusAudioComponent != nullptr)
	{
		Page -> FocusAudioComponent->Stop();
		Page->FocusAudioComponent->DestroyComponent();
		Page -> FocusAudioComponent = nullptr;
	}
}
