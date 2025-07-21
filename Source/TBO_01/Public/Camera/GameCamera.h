
#pragma once

#include "CoreMinimal.h"
#include "DistanceToTargetModifier.h"
#include "LenghtModifier.h"
#include "OffsetHandlerModifier.h"
#include "RotationModifier.h"
#include "SplineModifier.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/SpringArmModifier.h"
#include "GameCamera.generated.h"


class UOcclusionMaterialHandler;

UCLASS()
class TBO_01_API AGameCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Sets default values for this actor's properties*/
	AGameCamera();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category="Camera")
	UCameraComponent* GetCameraComponent();
	
	/** Attach the camera to an actor
	 * if the actor implement ITargetCameraActor Interface set the camera on the actor
	 */
	UFUNCTION(BlueprintCallable, Category="Camera")
	void Attach(AActor* Actor, bool ZeroLocation);

	/** Detach actor from parent and remove all modifier keep the camera position*/
	
	UFUNCTION(BlueprintCallable, Category="Camera")
	void Detach();
	
	/**call all the modifier revert with immediate = true */
	UFUNCTION(BlueprintCallable, Category="Camera")
	void ResetSpringArmToBegin();

	//Create and add Modifier
	/**Add offset to forward modifier if the passed modifier is in the list change only the parameter
	 *with no creation
	 */
	

	/**Add rotation Modifier
	 *multiple rotation modifier can be added*/

	UFUNCTION(blueprintCallable, Category="Camera")
	UObject*  AddRotationModifier(const float DeltaYaw,const float DeltaPitch,const float DeltaRoll,const float Duration);
	
	UFUNCTION(blueprintCallable, Category="Camera")
	UObject* AddLenghtModifier(float DeltaLenght, float Duration);

	

	UFUNCTION(blueprintCallable, Category="Camera")
	UObject* AddSplineModifier(USplineComponent* Spline, const FVector& Start, const FVector& End, bool UseStartEndPointDistance, bool
	                           AdaptSpringArmLenght, bool Dolly, bool DollyAligned, bool ForwardAligned, float RotationSpeed, float LenghtSpeed, float
	                           SplineMovementSpeed);
	UFUNCTION(blueprintCallable, Category="Camera")
	UObject* AddDistanceToTargetModifier(AActor* TargetActor, UCurveFloat* Curve, float TargetDeltaArmLength);


	//** Modifier Changer */
	UFUNCTION(BlueprintCallable, Category="Camera")
	void ChangeRotation(FRotator Rotation);
	UFUNCTION(BlueprintCallable, Category="Camera")
	void ChangeLenght(float Lenght);
	UFUNCTION(BlueprintCallable, Category="Camera")
	void SetLenghtToDefault();
	UFUNCTION(BlueprintCallable, Category="Camera")
	void SetRotationToDeafult();
	UFUNCTION(BlueprintCallable, Category="Camera")
	void RemoveSplineModifier(bool ResetRotation, bool ResetLenght);
	UFUNCTION(BlueprintCallable, Category="Camera")
	void RemoveDistanceToTargetModifier(bool ResetLength);

	/** offset Handle function */
	UFUNCTION(blueprintCallable, Category="Camera")
	void SetLockOnTarget(AActor* Target);
	UFUNCTION(blueprintCallable, Category="Camera")
	bool iSLockOnTargetSet();
	UFUNCTION(blueprintCallable, Category="Camera")
	void SetOffsetForwardTarget(AActor* Target);
	UFUNCTION(blueprintCallable, Category="Camera")
	void SetOffsetEnabled(bool Enabled,bool ImmediateZeroOffset);
	UFUNCTION(blueprintCallable, Category="Camera")
	void SetManualOffset(const FVector& Offset, bool UseDefaultOffset);


	UFUNCTION(blueprintCallable, Category="Camera")
	void SetAttachedActor(AActor* Actor);
	UFUNCTION(blueprintCallable, Category="Camera")
	AActor* GetAttachedActor() { return AttachedActor; }
	
	//Handle modifier array

	/**  revert the passed modifier if present in the array
	 *  immediate instant revert without interpolation
	 */
	UFUNCTION(BlueprintCallable, Category="Camera")
	void RevertModifier(UObject* Modifier,bool Immediate);
	/** Apply revert all modifier applied  */
	UFUNCTION(BlueprintCallable, Category="Camera")
	void RevertAllAppliedModifier(bool Immediate);

	UPROPERTY(BlueprintReadOnly,Category="Camera|Offset")
	TObjectPtr<UOffsetHandlerModifier> OffsetHandlerModifier;
	UPROPERTY(BlueprintReadOnly,Category="Camera|Rotation")
	TObjectPtr<URotationModifier>	RotationModifier;
	UPROPERTY(BlueprintReadOnly,Category="Camera|Lenght")
	TObjectPtr<ULenghtModifier> LenghtModifier;
	UPROPERTY(BlueprintReadOnly,Category="Camera|SplineModifier")
	TObjectPtr<USplineModifier> SplineModifier;
	UPROPERTY(BlueprintReadOnly,Category="Camera|SplineModifier")
	TObjectPtr<UDistanceToTargetModifier> DistanceToTargetModifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Offset", meta = (Group ="Offset Setting"))
	float InterpSpeed = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Offset", meta = (Group ="Offset Setting"))
	float LockOffset = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Offset", meta = (Group ="Offset Setting"))
	float ForwardOffset = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Offset", meta = (Group ="Offset Setting"))
	float MinLockDistance = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Offset", meta = (Group ="Offset Setting"))
	float MaxLockDistance = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|InterpSpeed", meta = (Group ="Offset Setting"))
	float CameraInterpSpeed = 5.0f;
	
protected:

	/** Properties */
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	TObjectPtr<AActor> AttachedActor;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	TObjectPtr<UOcclusionMaterialHandler> OcclusionMaterialHandler;


	//array of modifier currently applied to the camera
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	TArray<TScriptInterface<ISpringArmModifier>> SpringArmModifiers;



	



	UFUNCTION()
	UOffsetHandlerModifier* CreateDefaultOffsetHandlerModifier();
	UFUNCTION()
	URotationModifier* CreateDefaultRotationModifier();
	UFUNCTION()
	ULenghtModifier* CreateDefaultLenghtModifier();
	UFUNCTION()
	USplineModifier* CreateDefaultSplineModifier();
	UFUNCTION()
	UDistanceToTargetModifier* CreateDefaultDistanceToTargetModifier();
	UFUNCTION()
	void SetDefaultModifiers();

	UFUNCTION()
	void UpdateModifier(float DeltaTime);
	
	//default property saved on begin play
	FVector DefaultOffset;
	
	FRotator DefaultRotation;
	
	float DefaultArmLenght;

	float CurrentInterpSpeed = 0;

	
	//function
	virtual void BeginPlay() override;
	

	
	

};


