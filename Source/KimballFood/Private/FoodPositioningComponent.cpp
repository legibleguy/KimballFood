
#include "FoodPositioningComponent.h"
#include "KimballFoodIngredient.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogKimballFood, Log, All);

UFoodPositioningComponent::UFoodPositioningComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFoodPositioningComponent::OnIngredientAdded(UKimballFoodIngredient* Ingredient)
{
    if (Ingredient)
    {
        UE_LOG(LogKimballFood, Log, TEXT("Ingredient '%s' added to component '%s'"), *Ingredient->GetName(), *GetName());
    }
}
