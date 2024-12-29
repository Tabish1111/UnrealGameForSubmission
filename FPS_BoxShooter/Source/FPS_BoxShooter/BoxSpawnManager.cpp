#include "BoxSpawnManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

ABoxSpawnManager::ABoxSpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;

    // Setup components
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    // Load default mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }

    MeshComponent->SetRelativeScale3D(FVector(1.0f));
    TotalScore = 0;
    InitializeTestData();
}

void ABoxSpawnManager::InitializeTestData()
{
    TestColors.Add(FLinearColor(1.0f, 0.0f, 0.0f)); // Red
    TestColors.Add(FLinearColor(0.0f, 1.0f, 0.0f)); // Green
    TestColors.Add(FLinearColor(0.0f, 0.0f, 1.0f)); // Blue
    TestColors.Add(FLinearColor(1.0f, 1.0f, 0.0f)); // Yellow
}

void ABoxSpawnManager::BeginPlay()
{
    Super::BeginPlay();
}

AInteractiveBox* ABoxSpawnManager::SpawnSingleBox(const FVector& Location, const FRotator& Rotation, const FVector& Scale)
{
    AInteractiveBox* NewBox = GetWorld()->SpawnActor<AInteractiveBox>(AInteractiveBox::StaticClass(), Location, Rotation);

    if (NewBox)
    {
        // Set box properties
        int32 Health = FMath::RandRange(1, 3);
        int32 Score = 10;
        FLinearColor Color = TestColors[FMath::RandRange(0, TestColors.Num() - 1)];

        NewBox->SetBoxProperties(TEXT("TestBox"), Color, Health, Score);
        NewBox->OnBoxDestroyed.AddDynamic(this, &ABoxSpawnManager::OnBoxDestroyedHandler);

        // Scale the box
        NewBox->SetActorScale3D(Scale);

        SpawnedBoxes.Add(NewBox);
    }

    return NewBox;
}

void ABoxSpawnManager::SpawnBoxes(int32 NumberOfBoxes, float SpawnRadius)
{
    for (int32 i = 0; i < NumberOfBoxes; ++i)
    {
        FVector SpawnLocation = GetActorLocation()  + FMath::RandPointInCircle(SpawnRadius);
        SpawnSingleBox(SpawnLocation, FRotator::ZeroRotator, FVector(1.0f));
    }
}

void ABoxSpawnManager::OnBoxDestroyedHandler(int32 ScoreValue)
{
    TotalScore += ScoreValue;
    CleanupDestroyedBoxes();
}

void ABoxSpawnManager::CleanupDestroyedBoxes()
{
    // Cleanup boxes that have been destroyed
    SpawnedBoxes.RemoveAll([](AInteractiveBox* Box) { return !Box; });
}
