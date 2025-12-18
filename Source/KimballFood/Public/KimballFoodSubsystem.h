
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "KimballFoodSubsystem.generated.h"

class UFoodPositioningComponent;
class UKimballFoodIngredient;

/**
 * this subsystem manages all the food ingredients in the game
 */
UCLASS()
class KIMBALLFOOD_API UKimballFoodSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// gets the ingredient data asset for a given tag
	UFUNCTION(BlueprintCallable, Category = "Food")
	UKimballFoodIngredient* GetIngredientByTag(const FGameplayTag& IngredientTag);

	// mesh associated with a given ingredient tag
	UFUNCTION(BlueprintCallable, Category = "Food")
	TSoftObjectPtr<UStaticMesh> GetMeshForTag(const FGameplayTag& IngredientTag);

	// gets the positioning component class associated with a given ingredient tag
	TSoftClassPtr<UFoodPositioningComponent> GetPositioningComponentClassForTag(const FGameplayTag& IngredientTag);

	// all the unique meshes for all ingredients
	void GetAllUniqueMeshes(TArray<TSoftObjectPtr<UStaticMesh>>& OutMeshes);

	// the ingredient mesh map
	const TMap<FGameplayTag, TSoftObjectPtr<UStaticMesh>>& GetIngredientMeshMap() const { return IngredientMeshMap; }

private:
	// where we map tags to ingredient data assets after loading them
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UKimballFoodIngredient>> IngredientDataMap;

	// where we map tags to meshes after loading them
	UPROPERTY()
	TMap<FGameplayTag, TSoftObjectPtr<UStaticMesh>> IngredientMeshMap;

	// where we map tags to positioning component classes after loading them
	UPROPERTY()
	TMap<FGameplayTag, TSoftClassPtr<UFoodPositioningComponent>> IngredientPositioningComponentMap;
};
