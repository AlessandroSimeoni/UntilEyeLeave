// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineMeshInstancer.generated.h"

class UStaticMeshComponent;
class USplineComponent;
class UHierarchicalInstancedStaticMeshComponent;


USTRUCT(BlueprintType)
struct FMeshWithWeight
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 Weight = 1;
};

UCLASS()
class TBO_01_API ASplineMeshInstancer : public AActor
{
	GENERATED_BODY()

public:
	ASplineMeshInstancer();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* SplineComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instancing")
	TArray<FMeshWithWeight> Meshes;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instancing")
	float SectionLength = 500.0f;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instancing")
	FVector BoundingBoxSize = FVector(200, 200, 200);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instancing", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	int32 MinInstancesPerSection = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instancing", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	int32 MaxInstancesPerSection = 3;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instancing")
	int32 RandomSeed = 12345;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instancing")
	bool bRandomRotation = true;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instancing")
	bool bAlignToSpline = true;

	
	UPROPERTY()
	TMap<FIntPoint, UHierarchicalInstancedStaticMeshComponent*> MeshComponents;

	void ClearInstances();
	void GenerateInstances();
};
