#include "InteractiveBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

AInteractiveBox::AInteractiveBox()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create and setup the box mesh component
    BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
    RootComponent = BoxMesh;

    // Load the default cube static mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMeshAsset.Succeeded())
    {
        BoxMesh->SetStaticMesh(CubeMeshAsset.Object);
    }

    // Load the default material
    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
    if (MaterialAsset.Succeeded())
    {
        BoxMesh->SetMaterial(0, MaterialAsset.Object);
    }

    // Setup physics and collision
    BoxMesh->SetSimulatePhysics(true);
    BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

    // Default properties
    Health = 3;
    Score = 10;
    Color = FLinearColor(1.0f, 1.0f, 1.0f);
    Type = TEXT("Default");
}

void AInteractiveBox::BeginPlay()
{
    Super::BeginPlay();
    SetupBoxMesh();
}

void AInteractiveBox::SetupBoxMesh()
{
    if (BoxMesh && BoxMesh->GetMaterial(0))
    {
        DynamicMaterial = UMaterialInstanceDynamic::Create(BoxMesh->GetMaterial(0), this);
        BoxMesh->SetMaterial(0, DynamicMaterial);
        UpdateMaterialColor();
    }
}

void AInteractiveBox::UpdateMaterialColor()
{
    if (DynamicMaterial)
    {
        FLinearColor CurrentColor = Color;
        float HealthPercentage = static_cast<float>(Health) / 3.0f;
        CurrentColor.A = FMath::Max(0.3f, HealthPercentage);
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), CurrentColor);
    }
}

void AInteractiveBox::SetBoxProperties(const FString& InType, const FLinearColor& InColor, int32 InHealth, int32 InScore)
{
    Type = InType;
    Color = InColor;
    Health = InHealth;
    Score = InScore;
    UpdateMaterialColor();
}

void AInteractiveBox::PlayDestructionEffects()
{
    if (BoxMesh)
    {
        // Disable physics and collision
        BoxMesh->SetSimulatePhysics(false);
        BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        // Optional: Add particle effects, sounds, etc. here

        // Fade out the material
        if (DynamicMaterial)
        {
            FLinearColor DestroyedColor = Color;
            DestroyedColor.A = 0.0f;
            DynamicMaterial->SetVectorParameterValue(TEXT("Color"), DestroyedColor);
        }
    }
}

void AInteractiveBox::TakeDamage()
{
    Health--;

    if (Health > 0)
    {
        UpdateMaterialColor();
    }
    else
    {
        // Broadcast score before visual effects
        OnBoxDestroyed.Broadcast(Score);

        // Play destruction effects
        PlayDestructionEffects();

        // Delayed destruction to allow effects to play
        FTimerHandle DestroyTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DestroyTimerHandle,
            [this]()
            {
                Destroy();
            },
            0.2f,
            false
        );
    }
}
