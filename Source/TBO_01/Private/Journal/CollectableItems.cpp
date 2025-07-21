#include "Journal/CollectableItems.h"
#include <SaveSystem/UniqueIDComponent.h>
#include "Character/MainCharacter.h"
#include "Character/MainCharacter.h"
#include "Character/GAS/PlayerAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"

class USaveSubsystem;
class UTBOSaveGame;

ACollectableItems::ACollectableItems()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = MyRootComponent;

	CollectableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectableMesh"));
	CollectableMesh->SetupAttachment(MyRootComponent);

	SecondaryCollectableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondaryCollectableMesh"));
	SecondaryCollectableMesh->SetupAttachment(MyRootComponent);
	
	UniqueIDComponent = CreateDefaultSubobject<UUniqueIDComponent>(TEXT("UniqueID"));
	
	DetectionRange = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionRange"));
	DetectionRange->SetupAttachment(MyRootComponent);
	DetectionRange -> OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnDetectionRangeBeginOverlap);
	
	CollectionRange = CreateDefaultSubobject<UBoxComponent>(TEXT("CollectionRange"));
	CollectionRange->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollectionRange->SetupAttachment(MyRootComponent);

	InteractionPromptLocation = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionPromptLocation"));
	InteractionPromptLocation->SetupAttachment(MyRootComponent);
}

void ACollectableItems::Interact_Implementation()
{
	AMainCharacter* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!MainChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("No main character found"));
		return;
	}
	
	if (Price <= 0.0f)
	{
		PlayCollectableSFX();
		OnInteractSuccess();
		Collected();
		return;
	}

	if (UAbilitySystemComponent* ASC = MainChar->GetAbilitySystemComponent())
	{
		const UPlayerAttributeSet* AttrSet = ASC->GetSet<UPlayerAttributeSet>();
		if (AttrSet)
		{
			float PlayerCoins = AttrSet->GetCoin();

			if (PlayerCoins >= Price)
			{
				if (GE_SpendCoinsClass)
				{
					FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GE_SpendCoinsClass, 1.0f, ASC->MakeEffectContext());
					if (SpecHandle.IsValid())
					{
						const FGameplayTag CoinCostTag = FGameplayTag::RequestGameplayTag(FName("Currency.CoinCost"));
						SpecHandle.Data->SetSetByCallerMagnitude(CoinCostTag, -Price); 
						ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("GE not set"));
				}

				PlayPurchaseSFX();
				OnInteractSuccess();
				Collected();
			}
			else
			{
				OnInteractionFailed.Broadcast();
			}
		}
	}
}

USceneComponent* ACollectableItems::GetInteractionPromptSceneComponent_Implementation()
{
	return InteractionPromptLocation.Get();
}

void ACollectableItems::HandleCollectedStatus()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	ItemCollected = true;
}

void ACollectableItems::CheckSaveFile()
{
	if (TBOSaveGame->FindInteractedActor(UniqueIDComponent->UniqueID))
	{
		HandleCollectedStatus();
	}
}

void ACollectableItems::OnDetectionRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (!Character)
		return;
	Discovered();
	UE_LOG(LogTemp, Warning, TEXT("OnDetectionRangeBeginOverlap"));
}

void ACollectableItems::BeginPlay()
{
	Super::BeginPlay();

	SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	TBOSaveGame = SaveSubsystem->LoadGame();
	
	// delayed check
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(TH, this, &ThisClass::CheckSaveFile, 0.1f, false);
}

void ACollectableItems::Discovered()
{
	if (TBOSaveGame)
		TBOSaveGame -> AddDiscoverCollectible(this);
	
	ItemDiscovered = true;
}

void ACollectableItems::Collected()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	TBOSaveGame -> RemovedDiscoveredCollectible(this);
	TBOSaveGame->AddInteractedActor(UniqueIDComponent->UniqueID);
	ItemCollected = true;
	OnObjectCollected.Broadcast(this);
}

