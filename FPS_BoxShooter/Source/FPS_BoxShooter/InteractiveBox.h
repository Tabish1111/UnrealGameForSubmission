#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveBox.generated.h"

// Delegate to notify when the box is destroyed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoxDestroyed, int32, ScoreValue);

UCLASS()
class FPS_BOXSHOOTER_API AInteractiveBox : public AActor
{
    GENERATED_BODY()

public:
    AInteractiveBox();

    // Function to handle damage when the box is shot
    UFUNCTION(BlueprintCallable, Category = "Box Properties")
    void TakeDamage();

    // Set properties from outside (e.g., from a spawn manager or level logic)
    UFUNCTION(BlueprintCallable, Category = "Box Properties")
    void SetBoxProperties(const FString& InType, const FLinearColor& InColor, int32 InHealth, int32 InScore);

    // Getters for health and score (optional for Blueprint)
    UFUNCTION(BlueprintPure, Category = "Box Properties")
    int32 GetHealth() const { return Health; }

    UFUNCTION(BlueprintPure, Category = "Box Properties")
    int32 GetScore() const { return Score; }

protected:
    virtual void BeginPlay() override;

    // Properties of the box, editable in the editor and set via SetBoxProperties()
    UPROPERTY(EditAnywhere, Category = "Box Properties")
    FString Type;

    UPROPERTY(EditAnywhere, Category = "Box Properties")
    FLinearColor Color;

    UPROPERTY(EditAnywhere, Category = "Box Properties")
    int32 Health;

    UPROPERTY(EditAnywhere, Category = "Box Properties")
    int32 Score;

    // Components of the box
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BoxMesh;

    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterial;

    // Private methods for setup, destruction, and material updates
    void SetupBoxMesh();
    void UpdateMaterialColor();
    void PlayDestructionEffects();

public:
    // Delegate that gets called when the box is destroyed
    UPROPERTY(BlueprintAssignable, Category = "Box Events")
    FOnBoxDestroyed OnBoxDestroyed;
};
