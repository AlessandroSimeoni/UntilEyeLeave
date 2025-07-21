// Fill out your copyright notice in the Description page of Project Settings.


#include "Eyes/EyesHolderActor.h"

AEyesHolderActor::AEyesHolderActor()
{
	EyesBody = CreateDefaultSubobject<USkeletalMeshComponent>("EyeSkeletalMesh");
	EyesBody->SetCollisionProfileName(TEXT("EyesBody"));
	EyesHandlerActorComponent = CreateDefaultSubobject<UEyesHandlerActorComponent>("EyesHandlerActorComponent");
	
}

// Called when the game starts or when spawned
void AEyesHolderActor::BeginPlay()
{
	Super::BeginPlay();

	GetComponents<UEyeSceneComponent>(Eyes);
	EyesHandlerActorComponent->SetUpEyes(Eyes, EyesHitPoint,EyesLevel,EyesTransition,EyesBody);

	if (EyesHandlerActorComponent)
	{
		EyesHandlerActorComponent->OnEyesHit.AddDynamic(this, &AEyesHolderActor::OnEyeHit);
		EyesHandlerActorComponent->OnEyeDeath.AddDynamic(this, &AEyesHolderActor::OnEyeDeath);
		EyesHandlerActorComponent->OnAllEyesDeath.AddDynamic(this, &AEyesHolderActor::OnAllEyesDeath);
	}
}



UEyesHandlerActorComponent* AEyesHolderActor::GetEyesHandlerActorComponent_Implementation()
{
	return EyesHandlerActorComponent;
}
