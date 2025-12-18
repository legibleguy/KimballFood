
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GASEssentialsCharacter/Public/GASActor.h"
#include "FoodActor.generated.h"

class UInstancedStaticMeshComponent;
class UFoodPositioningComponent;
class UStaticMesh;

/**
 * an actor that visually represents a stack of food ingredients
 */
UCLASS()
class KIMBALLFOOD_API AFoodActor : public AGASActor
{
	GENERATED_BODY()

public:
	AFoodActor();

protected:
	virtual void BeginPlay() override;

public:
	// call this to add a new ingredient to the stack
	UFUNCTION(BlueprintCallable, Category = "Food")
	void AddIngredient(const FGameplayTag& IngredientTag);

	// clears all ingredients and resets the actor
	UFUNCTION(BlueprintCallable, Category = "Food")
	void ClearFood();

	// gets the mesh-to-component map
	const TMap<TSoftObjectPtr<UStaticMesh>, TObjectPtr<UInstancedStaticMeshComponent>>& GetMeshToComponentMap() const { return MeshToComponentMap; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIngredientAdded, const FGameplayTag&, IngredientTag);

	UPROPERTY(BlueprintAssignable, Category = "Food")
	FOnIngredientAdded OnIngredientAdded;

private:
	// just a simple root so we can move the actor around
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneRoot;

	// the active positioning component, if one has been enforced by an ingredient
	UPROPERTY()
	TObjectPtr<UFoodPositioningComponent> PositioningComponent;

	// keeps track of the ingredients we've added in order
	UPROPERTY(VisibleInstanceOnly, Category = "Food State")
	TArray<FGameplayTag> AddedIngredients;

	// maps a mesh to the ismc that handles it
	UPROPERTY()
	TMap<TSoftObjectPtr<UStaticMesh>, TObjectPtr<UInstancedStaticMeshComponent>> MeshToComponentMap;

	// for the simple stacking logic, we'll just track the height
	float CurrentStackHeight = 0.0f;

	UPROPERTY()
	TObjectPtr<UStaticMesh> LastIngredientMesh;

	FTransform LastInstanceTransform;
};
