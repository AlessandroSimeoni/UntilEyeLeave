#include "Eyes/EyeSceneComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TimelineComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Lifesystem/LifeSystemComponent.h"
#include "Curves/CurveFloat.h"
#include "UObject/FastReferenceCollector.h"
#include "Character/MainCharacter.h"
// Create and setup all necessary components
UEyeSceneComponent::UEyeSceneComponent()
{
	//Create and attach EyeMesh
	EyeMesh = CreateDefaultSubobject<USkeletalMeshComponent>("EyeSkeletalMesh");
	EyeMesh->SetCollisionProfileName(TEXT("Pawn"));
	
	//Create and attach the RangeBox
	EyeRangeBox = CreateDefaultSubobject<UBoxComponent>("EyeRangeBox");
	EyeRangeBox->SetCollisionProfileName(TEXT("Custom"));
	EyeRangeBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EyeRangeBox->SetCollisionObjectType(ECC_WorldDynamic);
	EyeRangeBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	
	
	//Create and attach the Hitbox
	EyeHitBox = CreateDefaultSubobject<UBoxComponent>("EyeHitBox");
	EyeHitBox->SetCollisionProfileName(TEXT("UI"));


	
	//Create and attach the LifeComponent
	EyeLife = CreateDefaultSubobject<ULifeSystemComponent>("EyeLife");
	
	
	
	//Set up default config parameters for the components
	MeshScale = FVector(1.0f, 1.0f, 1.0f);
	RangeBoxScale = FVector(20.0f, 20.0f, 20.0f);
	HitBoxScale = FVector(10.0f, 10.0f, 10.0f);
	RangeBoxRelativeLocation = FVector(10.0f, 10.0f, 0.0f);
	HitBoxRelativeLocation = FVector(10.0f, 10.0f, 0.0f);
	EyeOffsetRotation = FRotator(0, 0, 0);
	//TODO implements the creature connected to the eye
	//ControllerCreature = Cast<ACharacter>(GetAttachmentRootActor());
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	//EyeMesh->SetComponentTickEnabled(false);
}

void UEyeSceneComponent::BeginPlay()
{
	Super::BeginPlay();
	OriginalEyeMeshScale = EyeMesh->GetRelativeScale3D();
	//Bind events 
	EyeRangeBox-> OnComponentBeginOverlap.AddDynamic(this, &UEyeSceneComponent::OnOverlapStartRange);
	EyeRangeBox-> OnComponentEndOverlap.AddDynamic(this, &UEyeSceneComponent::OnOverlapEndRange);
	//EyeHitBox -> OnComponentBeginOverlap.AddDynamic(this, &UEyeSceneComponent::OnOverlapStartHit);
	EyeLife ->OnDeath.AddDynamic(this, &UEyeSceneComponent::NotifyEyeDeath);
	//EyeHitBox -> SetActive(false);
	//ControllerCreature -> OnExternalInput.AddDynamic(this, &UEyeSceneComponent::OpenCloseEye);

	//set default rotation
	DefaultQuat =  EyeMesh->GetRelativeRotation().Quaternion();
	DefaultRotator = EyeMesh->GetRelativeRotation();
	EyeMesh->SetComponentTickEnabled(false);
	
	//set timeline function to lerp material
}

void UEyeSceneComponent::OnRegister()
{
	//Set the components values 
	Super::OnRegister();
	if (EyeMesh)
	{
		//Create material interface to lerp
		EyeMesh ->SetRelativeScale3D(MeshScale);
			UMaterialInterface* BaseMaterial = EyeMaterial;
		if (BaseMaterial)
		{
			DynamicEyeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			EyeMesh->SetMaterial(0, DynamicEyeMaterial);
		}
	}

	SetAttachment();
}

#if WITH_EDITOR
void UEyeSceneComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif

void UEyeSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleVibration(DeltaTime);
	if (IsEyeRotationTransition)
	{
		if (EyeTarget && EyeOpen)
		{
			LookTarget(DeltaTime);
		}
		else if (!EyeTarget)
		{
			ResetEyeRotation(DeltaTime);
			
		}
	}
	
	 if (IsEyeVulnerableTransition)
	{
		VulnerableTransition(DeltaTime);
	 }
	/*
	if (!IsEyeRotationTransition && !IsEyeVulnerableTransition)
	{
		//SetComponentTickEnabled(false);
		//EyeMesh->SetComponentTickEnabled(false);
	}
	*/

}

bool UEyeSceneComponent::Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator,float Intensity)
{
	float DamageReceived = 0.0f;
	LastInstigator = AttackInstigator;

	
	if (EyeLevel <= CharacterLevel && EyeOpen)
	{
		EyeLife->ReceiveDamage(DamageValue);
		StartVibration();
		DamageReceived = DamageValue;
	}
	OnEyeHit.Broadcast(this,DamageReceived,AttackInstigator,1);

	return DamageReceived != 0.0f;
}

AActor* UEyeSceneComponent::GetEyeTarget()
{
	if (EyeTarget) return EyeTarget;
	else return nullptr;
}

void UEyeSceneComponent::LookTarget(float DeltaTime)
{
	if (!EyeMesh || !EyeTarget) return; 

	FVector EyeLocation = EyeMesh->GetComponentLocation();
	FVector TargetLocation = EyeTarget->GetActorLocation();

	
	FRotator WorldTargetRotation = UKismetMathLibrary::FindLookAtRotation(EyeLocation, TargetLocation);
	WorldTargetRotation += EyeOffsetRotation;
	WorldTargetRotation.Roll = 0.0f;

	FTransform ParentTransform = EyeMesh->GetAttachParent()->GetComponentTransform();
	FRotator LocalTargetRotation = (ParentTransform.InverseTransformRotation(WorldTargetRotation.Quaternion())).Rotator();
	
	LocalTargetRotation.Pitch = FMath::Clamp(LocalTargetRotation.Pitch, MinPitch, MaxPitch);
	LocalTargetRotation.Yaw = FMath::Clamp(LocalTargetRotation.Yaw, MinYaw, MaxYaw);

	
	FRotator ClampedWorldRotation = ParentTransform.TransformRotation(LocalTargetRotation.Quaternion()).Rotator();

	FVector ForwardVector = ClampedWorldRotation.Vector() * 100.0f;

	DrawDebugLine(
		GetWorld(),
		EyeLocation,
		EyeLocation + ForwardVector,
		FColor::Red,
		false,
		0.1f,
		0,
		2.0f
	);

	FQuat CurrentQuat = EyeMesh->GetComponentQuat();
	FQuat TargetQuat = ClampedWorldRotation.Quaternion();

	FQuat NewQuat = FQuat::Slerp(CurrentQuat, TargetQuat, DeltaTime * EyeRotationSpeed);

	EyeMesh->SetWorldRotation(NewQuat);
}



void UEyeSceneComponent::ResetEyeRotation(float DeltaTime)
{
	if (!EyeMesh) return;
	
	FRotator CurrentRotation = EyeMesh->GetRelativeRotation();

	if (CurrentRotation.Equals(DefaultRotator, 0.01f)) 
	{
		EyeMesh->SetRelativeRotation(DefaultRotator);
		IsEyeRotationTransition = false;
		return;
	}
	
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DefaultRotator, DeltaTime, EyeRotationSpeed);

	EyeMesh->SetRelativeRotation(NewRotation);
	
}


void UEyeSceneComponent::VulnerableTransition(float DeltaTime)
{
	if (!DynamicEyeMaterial || !EyeMesh)
	{
		IsEyeVulnerableTransition = false;
		return;
	}

	if (EyeLerpCurve)
	{
		VulnerableTransitionAlpha = FMath::FInterpTo(VulnerableTransitionAlpha, TargetTransitionAlpha, DeltaTime, EyeVulnerableTransitionSpeed);
	}
	else
	{
		VulnerableTransitionAlpha = FMath::Lerp(VulnerableTransitionAlpha, TargetTransitionAlpha, DeltaTime * EyeVulnerableTransitionSpeed);
	}

	float CurveValue = EyeLerpCurve ? EyeLerpCurve->GetFloatValue(VulnerableTransitionAlpha) : VulnerableTransitionAlpha;

	DynamicEyeMaterial->SetScalarParameterValue("BlendAlpha", CurveValue);
	EyeMesh->SetMorphTarget(EyeSlitMorphTarget, CurveValue);

	//UE_LOG(LogTemp,Warning,TEXT("VulnerableTransitionAlpha = %f end %s"),CurveValue, *EyeSlitMorphTarget.ToString());
	if (FMath::IsNearlyEqual(VulnerableTransitionAlpha, TargetTransitionAlpha, 0.01f))
	{
		IsEyeVulnerableTransition = false;
	}
}

void UEyeSceneComponent::OnOverlapStartRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//check if other actor exist
	if (!OtherActor || !OtherComp) return;
	//cast the actor to the main character



		SetEyeTarget(OtherActor);
	
}

void UEyeSceneComponent::SetEyeTarget(AActor* OtherActor)
{
	if (!IsValid(OtherActor)) return;

	
	AMainCharacter* MainCharacter = nullptr;
	if (OtherActor -> IsA(AMainCharacter::StaticClass()))
		MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter && MainCharacter -> Controller ->IsPlayerController())
	{
		
		//SetComponentTickEnabled(true);
		//EyeMesh->SetComponentTickEnabled(true);
		//check for weapon level and change material
		EyeTarget = OtherActor;
		IsEyeRotationTransition = true;
		
		
			if (MainCharacter->CurrentWeaponLevel >= EyeLevel)
				EyeVulnerable = true;
		
		if(!EyeVulnerable)
		{
			UE_LOG(LogTemp, Display, TEXT("EyeNotVulnerable"));
			IsEyeVulnerableTransition = true;
			TargetTransitionAlpha = 1.0f;
		}
	}
}

void UEyeSceneComponent::RemoveEyeTarget(AActor* OtherActor)
{
	if (!IsValid(OtherActor)) return;
	IsEyeRotationTransition = true;
	AMainCharacter* MainCharacter = nullptr;
	if (OtherActor -> IsA(AMainCharacter::StaticClass()))
		MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		EyeTarget = nullptr;
		IsEyeVulnerableTransition = true;
		TargetTransitionAlpha = 0;
	}
}


void UEyeSceneComponent::OnOverlapEndRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Exit eye range, clean target pointer
	if (!OtherActor || !OtherComp) return; // Avoid null pointer crashes
	RemoveEyeTarget(OtherActor);
}


ULifeSystemComponent* UEyeSceneComponent::GetLifeComponent()
{
	return EyeLife;
}


void UEyeSceneComponent::ChangeAyesHurtbox(bool IsHittable)
{
	EyeOpen = IsHittable;

	
	if (IsHittable)
	{
		UE_LOG(LogTemp, Warning, TEXT("EYe Open"));
	EyeHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}
	else
	{
	EyeHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
	
}

void UEyeSceneComponent::NotifyEyeDeath(USceneComponent* Self)
{
	
	if (bIsDestroyed) return;
	bIsDestroyed = true;
	OnDeath.Broadcast(this,LastInstigator);
	EffectData.SpawnEffect(GetWorld(),EyeHitBox->GetComponentLocation());
	
	LastInstigator = nullptr;
	if (EyeRangeBox)
	{
		EyeRangeBox->SetActive(false);
		EyeRangeBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EyeRangeBox->SetHiddenInGame(true);
	}

	if (EyeHitBox)
	{
		EyeHitBox->SetActive(false);
		EyeHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EyeHitBox->SetHiddenInGame(true);
	}

	if (EyeLife)
	{
		EyeLife->SetActive(false);
		EyeLife->SetHiddenInGame(true);
	}

	if (EyeMesh)
	{
		EyeMesh->SetActive(false);
		EyeMesh->SetVisibility(false, true); 
		EyeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	
	EyeTarget = nullptr;
	
	//SetComponentTickEnabled(false);
}

void UEyeSceneComponent::RestoreEyeComponent()
{
	if (!bIsDestroyed) return;
	bIsDestroyed = false;
	
	if (EyeRangeBox)
	{
		EyeRangeBox->SetActive(true);
		EyeRangeBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EyeRangeBox->SetHiddenInGame(false);
	}

	if (EyeHitBox)
	{
		EyeHitBox->SetActive(true);
		EyeHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EyeHitBox->SetHiddenInGame(false);
	}

	if (EyeLife)
	{
		EyeLife->SetActive(true);
		EyeLife->SetHiddenInGame(false);
		EyeLife->ResetCurrentHealth();
	}

	if (EyeMesh)
	{
		EyeMesh->SetActive(true);
		EyeMesh->SetVisibility(true, true);
		EyeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	
}


void UEyeSceneComponent::SetAttachment()
{
    // EyeMesh
    if (!EyeMesh)
    {
        //EyeMesh = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), TEXT("EyeSkeletalMesh"));
        //EyeMesh->RegisterComponent();
    }
    if (EyeMesh && EyeMesh->GetAttachParent() != this)
    {
        EyeMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    }

    // EyeRangeBox
    if (!EyeRangeBox)
    {
      //  EyeRangeBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("EyeRangeBox"));
       // EyeRangeBox->RegisterComponent();

    }
    if (EyeRangeBox && EyeRangeBox->GetAttachParent() != this)
    {
        EyeRangeBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    }

   
    if (!EyeHitBox)
    {
      //  EyeHitBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("EyeHitBox"));
       // EyeHitBox->RegisterComponent();
    }
    if (EyeHitBox && EyeHitBox->GetAttachParent() != this)
    {
        EyeHitBox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    }

    // EyeLife
    if (!EyeLife)
    {
      //  EyeLife = NewObject<ULifeSystemComponent>(this, ULifeSystemComponent::StaticClass(), TEXT("EyeLife"));
      //  EyeLife->RegisterComponent();
    }
    if (EyeLife && EyeLife->GetAttachParent() != this)
    {
        EyeLife->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    }
}

void UEyeSceneComponent::StartVibration()
{
	BounceStartTime = GetWorld()->GetTimeSeconds();
	bIsBouncing = true;
}

void UEyeSceneComponent::HandleVibration(float DeltaTime)
{
	if (bIsBouncing)
	{
		float ElapsedTime = GetWorld()->GetTimeSeconds() - BounceStartTime;

		if (ElapsedTime < BounceDuration)
		{
			float ScaleOffset = FMath::Sin(ElapsedTime * BounceFrequency) * FMath::Exp(-ElapsedTime * BounceDamping) * BounceAmplitude;
			
			float XYSquash = 1.0f + ScaleOffset;
			float ZStretch = 1.0f - ScaleOffset;

			FVector NewScale = OriginalEyeMeshScale * FVector(XYSquash, XYSquash, ZStretch);
			EyeMesh->SetRelativeScale3D(NewScale);
		}
		else
		{
			bIsBouncing = false;
			EyeMesh->SetRelativeScale3D(OriginalEyeMeshScale);
		}
	}
}