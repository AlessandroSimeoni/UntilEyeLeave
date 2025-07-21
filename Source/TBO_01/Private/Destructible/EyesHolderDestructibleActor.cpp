// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible/EyesHolderDestructibleActor.h"

AEyesHolderDestructibleActor::AEyesHolderDestructibleActor()
{
	EyesBody = CreateDefaultSubobject<USkeletalMeshComponent>("EyeSkeletalMesh");
	EyesBody->SetCollisionProfileName(TEXT("EyesBody"));
	EyesHandlerActorComponent = CreateDefaultSubobject<UEyesHandlerActorComponent>("EyesHandlerActorComponent");
	
}

// Called when the game starts or when spawned
void AEyesHolderDestructibleActor::BeginPlay()
{
	Super::BeginPlay();

	GetComponents<UEyeSceneComponent>(Eyes);
	EyesHandlerActorComponent->SetUpEyes(Eyes, EyesHitPoint,EyesLevel,EyesTransition,EyesBody);

	
	if (EyesHandlerActorComponent)
	{
		EyesHandlerActorComponent->OnEyesHit.AddDynamic(this, &AEyesHolderDestructibleActor::OnEyeHit);
		EyesHandlerActorComponent->OnEyeDeath.AddDynamic(this, &AEyesHolderDestructibleActor::OnEyeDeath);
		EyesHandlerActorComponent->OnAllEyesDeath.AddDynamic(this, &AEyesHolderDestructibleActor::HandleEyesDeath);
	}
}

void AEyesHolderDestructibleActor::HandleEyesDeath(UEyeSceneComponent* Eye,const AActor* ActorInstigator)
{
	Destruction();
	OnAllEyesDeath(Eye, ActorInstigator);
}


UEyesHandlerActorComponent* AEyesHolderDestructibleActor::GetEyesHandlerActorComponent_Implementation()
{
	return EyesHandlerActorComponent;
}
