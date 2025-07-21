#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "Interface/HittableActor.h"
#include "Lifesystem/LifeBridgeInterface.h"
#include "Utility/EffectData.h"
#include "EyeSceneComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathEvent, UEyeSceneComponent*, This,const AActor*,Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnEyeHit, UEyeSceneComponent*, This, float, DamageValue,const AActor*,Instigator,float,Force);

UCLASS( ClassGroup=(Eyes), meta=(BlueprintSpawnableComponent) )
class TBO_01_API UEyeSceneComponent : public USceneComponent, public ILifeBridgeInterface, public IHittableActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Eye")
	FOnDeathEvent OnDeath;
	UPROPERTY(BlueprintAssignable, Category="Eye")
	FOnEyeHit OnEyeHit;
	
	UEyeSceneComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Mesh")
	class USkeletalMeshComponent* EyeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Range")
	class UBoxComponent* EyeRangeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Hitbox")
	class UBoxComponent* EyeHitBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Life")
	class ULifeSystemComponent* EyeLife;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Mesh")
	FVector MeshScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Range")
	FVector RangeBoxScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Hitbox")
	FVector HitBoxScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Range")
	FVector RangeBoxRelativeLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye | Hitbox")
	FVector HitBoxRelativeLocation;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye")
	UMaterialInstance* EyeMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye")
	UCurveFloat* EyeLerpCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye")
	float EyeVulnerableTransitionSpeed = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye")
	float MaxAngleDegrees = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye")
	FName EyeSlitMorphTarget = "EyeSlit";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye")
	float EyeOpenTime =1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye", Meta = (UIMin = 1, ClampMin = 1))
	int EyeLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye")
	float EyeRotationSpeed = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye")
	FRotator EyeOffsetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|HitFeedback")
	float BounceDuration = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|HitFeedback")
	float BounceFrequency = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|HitFeedback")
	float BounceDamping = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|HitFeedback")
	float BounceAmplitude = 1.0f;   

	
	float BounceStartTime;
	bool bIsBouncing = false;
	FVector OriginalEyeMeshScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|FX")
	FEffectData EffectData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|Rotation Constraint")
	float MinPitch = -30.0f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|Rotation Constraint")
	float MaxPitch = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|Rotation Constraint")
	float MinYaw = -45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eye|Rotation Constraint")
	float MaxYaw = 45.0f;
	

	
private:
	bool EyeOpen = true;
	bool EyeVulnerable = false;
	bool bIsDestroyed = false;

	//Tick Influencer
	bool IsEyeVulnerableTransition = false;
	float IsEyeRotationTransition = false; 

	float TargetTransitionAlpha = 0.0f;
	float VulnerableTransitionAlpha = 0.0f;
	
	UPROPERTY()
	AActor* EyeTarget = nullptr;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicEyeMaterial;
	//UPROPERTY(EditAnywhere, Category="Eye")
	//ACharacter* ControllerCreature;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	AActor* GetEyeTarget();
	
	UFUNCTION()
	virtual ULifeSystemComponent* GetLifeComponent() override;
	virtual bool Hit_Implementation(int CharacterLevel, float DamageValue, AActor* AttackInstigator, float Intensity = 1.f) override;

	UFUNCTION(Blueprintable, Category="Eye")
	void ChangeAyesHurtbox(bool IsHittable);

	UFUNCTION(Blueprintable, Category="Eye")
	void SetEyeTarget(AActor* OtherActor);

	UFUNCTION(Blueprintable, Category="Eye")
	void RemoveEyeTarget(AActor* OtherActor);
	
	UFUNCTION(BlueprintCallable,Category="Eye")
	void RestoreEyeComponent();

	UFUNCTION(BlueprintCallable,Category="Eye")
	bool IsEyeDestroyed(){return bIsDestroyed;}
	
private:
	void LookTarget(float DeltaTime);
	void ResetEyeRotation(float DeltaTime);
	UPROPERTY()
	TObjectPtr<AActor>LastInstigator;
	UFUNCTION()
	void VulnerableTransition(float DeltaTime);
	UFUNCTION()
	void OnOverlapStartRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//void OnOverlapStartHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEndRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void NotifyEyeDeath(USceneComponent* Self);
	UFUNCTION()
	void SetAttachment();
	UFUNCTION()
	void StartVibration();
	UFUNCTION()
	void HandleVibration(float DeltaTime);

	FQuat DefaultQuat;
	FRotator DefaultRotator;
};

