// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/EyeButton.h"

#include "Character/MainCharacter.h"
#include "Components/BoxComponent.h"

AEyeButton::AEyeButton()
{	
	SocketMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketMeshComponent"));
	SocketMeshComponent->SetupAttachment(RootComponent);

	EyeHandlerActorComponent = CreateDefaultSubobject<UEyesHandlerActorComponent>("EyesHandlerActorComponent");

	EyeBody = CreateDefaultSubobject<USkeletalMeshComponent>("EyeSkeletalMesh");
	EyeBody->SetupAttachment(RootComponent);
	
}

void AEyeButton::BeginPlay()
{
	Super::BeginPlay();

	EyeBody->SetComponentTickEnabled(false);
	GetComponents<UEyeSceneComponent>(Eyes);
	EyeHandlerActorComponent->SetUpEyes(Eyes, 1,1,0.2f,EyeBody);

	if (EyeHandlerActorComponent)
		EyeHandlerActorComponent->OnEyesHit.AddDynamic(this, &ThisClass::HandleEyeHit);

	GetWorld()->GetTimerManager().SetTimer(TH, this, &ThisClass::HandleAlreadyOverlappingPlayer, 0.2f, false);
}

void AEyeButton::HandleAlreadyOverlappingPlayer()
{
	TArray<AActor*> OverlappingMainCharacter;
	for (const auto Eye : Eyes)
	{
		Eye->EyeRangeBox->GetOverlappingActors(OverlappingMainCharacter, AMainCharacter::StaticClass());
		if (OverlappingMainCharacter.Num() > 0)
			Eye->SetEyeTarget(OverlappingMainCharacter[0]);
	}
}

void AEyeButton::HandleEyeHit(UEyeSceneComponent* TargetEyeComponent, float Damage, const AActor* HitInstigator, float Force)
{	
	if (!Activated)
	{
		ActionActors();
	}
	else
	{
		DeActivateActors();
	}
}

UEyesHandlerActorComponent* AEyeButton::GetEyesHandlerActorComponent_Implementation()
{
	return EyeHandlerActorComponent;
}
