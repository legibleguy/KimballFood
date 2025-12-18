
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FoodPositioningComponent.generated.h"

class UKimballFoodIngredient;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class KIMBALLFOOD_API UFoodPositioningComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFoodPositioningComponent();

    virtual void OnIngredientAdded(UKimballFoodIngredient* Ingredient);
};
