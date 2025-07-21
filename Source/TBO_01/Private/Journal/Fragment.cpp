#include "Journal/Fragment.h"
#include "GISubsystem/CustomGameInstance.h"
#include "Journal/JournalManager.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/UniqueIDComponent.h"

AFragment::AFragment()
{
	PrimaryActorTick.bCanEverTick = false;
	Section = EPageType::Bestiary;
}

void AFragment::BeginPlay()
{
	Super::BeginPlay();
	JournalManager = Cast<AJournalManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AJournalManager::StaticClass()));
}

// Super collected e 
void AFragment::PickUpFragment()
{
	//OnFragmentCollected.Broadcast(RepresentedEntryID, Section, FragmentsIndexes, IsTrigger);
	if (JournalManager)
	{
		JournalManager->CollectFragment(RepresentedEntryID, Section, FragmentsIndexes, IsTrigger);
	}
}

void AFragment::Collected()
{
	PickUpFragment();
	Super::Collected();
}

void AFragment::Discovered()
{
	Super::Discovered();
}

