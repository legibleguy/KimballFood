
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "FoodPositioningComponent.h"
#include "KimballFoodIngredient.generated.h"

/**
 * data asset for a single food ingredient
 */
UCLASS(BlueprintType)
class KIMBALLFOOD_API UKimballFoodIngredient : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// the tag that uniquely identifies this ingredient
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient")
	FGameplayTag IngredientTag;

	// the mesh to use when this ingredient is in the world
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient")
	TSoftObjectPtr<UStaticMesh> Mesh;

	// an optional positioning component that may be added to the food actor when this ingredient is added
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient")
	TSoftClassPtr<UFoodPositioningComponent> PositioningComponentClass;
};
