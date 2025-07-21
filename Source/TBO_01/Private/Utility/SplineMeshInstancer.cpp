
#include "Utility/SplineMeshInstancer.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"

ASplineMeshInstancer::ASplineMeshInstancer()
{
    PrimaryActorTick.bCanEverTick = false;
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = SplineComponent;
}

void ASplineMeshInstancer::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    ClearInstances();
    GenerateInstances();
}

void ASplineMeshInstancer::ClearInstances()
{
    for (auto& Pair : MeshComponents)
    {
        if (Pair.Value)
        {
            Pair.Value->ClearInstances();
        }
    }
}

void ASplineMeshInstancer::GenerateInstances()
{
    if (Meshes.Num() == 0) return;

    FRandomStream RandomStream(RandomSeed);

    const float SplineLength = SplineComponent->GetSplineLength();
    const int32 NumSections = FMath::CeilToInt(SplineLength / SectionLength);

    for (int32 SectionIndex = 0; SectionIndex < NumSections; ++SectionIndex)
    {
        const float StartDistance = SectionIndex * SectionLength;
        const float EndDistance = FMath::Min((SectionIndex + 1) * SectionLength, SplineLength);
        const FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);
        const FVector EndLocation = SplineComponent->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::World);
        const FVector CenterLocation = (StartLocation + EndLocation) / 2;

        const int32 NumInstances = RandomStream.RandRange(MinInstancesPerSection, MaxInstancesPerSection);

      
        int32 TotalWeight = 0;
        for (const FMeshWithWeight& MeshWithWeight : Meshes)
        {
            if (MeshWithWeight.Mesh)
            {
                TotalWeight += MeshWithWeight.Weight;
            }
        }

        for (int32 i = 0; i < NumInstances; ++i)
        {
            if (TotalWeight <= 0) continue;

           
            int32 RandomValue = RandomStream.RandRange(1, TotalWeight);
            int32 AccumulatedWeight = 0;
            UStaticMesh* SelectedMesh = nullptr;

            for (const FMeshWithWeight& MeshWithWeight : Meshes)
            {
                if (!MeshWithWeight.Mesh) continue;

                AccumulatedWeight += MeshWithWeight.Weight;
                if (RandomValue <= AccumulatedWeight)
                {
                    SelectedMesh = MeshWithWeight.Mesh;
                    break;
                }
            }

            if (!SelectedMesh) continue;

            FIntPoint Key(SectionIndex, SelectedMesh->GetUniqueID());
            UHierarchicalInstancedStaticMeshComponent* InstComp = nullptr;

            if (!MeshComponents.Contains(Key))
            {
                InstComp = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
                if (!InstComp) continue;

                InstComp->RegisterComponent();
                InstComp->SetStaticMesh(SelectedMesh);
                InstComp->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

                InstComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                InstComp->bEnableDensityScaling = true;
                InstComp->SetMobility(EComponentMobility::Static);

                MeshComponents.Add(Key, InstComp);
            }
            else
            {
                InstComp = MeshComponents[Key];
            }

            if (!InstComp) continue;

            const FVector RandomOffset = FVector(
                RandomStream.FRandRange(-BoundingBoxSize.X / 2, BoundingBoxSize.X / 2),
                RandomStream.FRandRange(-BoundingBoxSize.Y / 2, BoundingBoxSize.Y / 2),
                RandomStream.FRandRange(-BoundingBoxSize.Z / 2, BoundingBoxSize.Z / 2)
            );

            FVector SpawnLocation = CenterLocation + RandomOffset;

            FQuat SpawnQuat = FQuat::Identity;
            if (bAlignToSpline)
            {
                FVector TangentDir = SplineComponent->GetDirectionAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);
                FQuat TangentQuat = FQuat::FindBetweenNormals(FVector::ForwardVector, TangentDir);
                SpawnQuat = TangentQuat;

                if (bRandomRotation)
                {
                    float RandomPitch = RandomStream.FRandRange(-180.0f, 180.0f);
                    float RandomRoll = RandomStream.FRandRange(-180.0f, 180.0f);
                    FQuat RandomQuat = FQuat(FRotator(RandomPitch, 0.0f, RandomRoll));
                    SpawnQuat = RandomQuat * SpawnQuat;
                }
            }
            else
            {
                if (bRandomRotation)
                {
                    float RandomYaw = RandomStream.FRandRange(0.0f, 360.0f);
                    float RandomPitch = RandomStream.FRandRange(-180.0f, 180.0f);
                    float RandomRoll = RandomStream.FRandRange(-180.0f, 180.0f);
                    SpawnQuat = FQuat(FRotator(RandomPitch, RandomYaw, RandomRoll));
                }
            }

            FTransform WorldTransform(SpawnQuat, SpawnLocation, FVector::OneVector);
            FTransform LocalTransform = WorldTransform.GetRelativeTransform(SplineComponent->GetComponentTransform());

            InstComp->AddInstance(LocalTransform);
        }
    }
}
