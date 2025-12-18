
#pragma once

#include "CoreMinimal.h"
#include "FoodPositioningComponent.h"
#include "GameplayTagContainer.h"
#include "StackingPositioningComponent.generated.h"

/**
 * a simple positioning component that stacks ingredients on top of each other
 */
UCLASS()
class KIMBALLFOOD_API UStackingPositioningComponent : public UFoodPositioningComponent
{
	GENERATED_BODY()

public:
	UStackingPositioningComponent();

	virtual void OnIngredientAdded(UKimballFoodIngredient* Ingredient) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stacking Rules")
	FGameplayTag BottomBunTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stacking Rules")
	FGameplayTag TopBunTag;

private:
	void RebuildStack();

	UPROPERTY()
	TArray<TObjectPtr<UKimballFoodIngredient>> MiddleIngredients;

	UPROPERTY()
	TObjectPtr<UKimballFoodIngredient> BottomBun;

	UPROPERTY()
	TObjectPtr<UKimballFoodIngredient> TopBun;
};
