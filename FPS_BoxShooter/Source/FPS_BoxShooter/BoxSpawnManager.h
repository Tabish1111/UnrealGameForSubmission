#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveBox.h"
#include "BoxSpawnManager.generated.h"

UCLASS()
class FPS_BOXSHOOTER_API ABoxSpawnManager : public AActor
{
    GENERATED_BODY()

public:
    ABoxSpawnManager();

    UFUNCTION(BlueprintCallable, Category = "Box Spawning")
    AInteractiveBox* SpawnSingleBox(const FVector& Location, const FRotator& Rotation, const FVector& Scale);

    UFUNCTION(BlueprintCallable, Category = "Box Spawning")
    void SpawnBoxes(int32 NumberOfBoxes, float SpawnRadius = 1000.0f);

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetTotalScore() const { return TotalScore; }

    UFUNCTION(BlueprintPure, Category = "Box Spawning")
    int32 GetActiveBoxCount() const { return SpawnedBoxes.Num(); }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Box Spawning")
    TArray<FLinearColor> TestColors;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY()
    TArray<AInteractiveBox*> SpawnedBoxes;

    UPROPERTY(VisibleAnywhere, Category = "Score")
    int32 TotalScore;

    void InitializeTestData();
    void CleanupDestroyedBoxes();

    UFUNCTION()
    void OnBoxDestroyedHandler(int32 ScoreValue);
};
