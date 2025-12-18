
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KimballFoodDataAsset.generated.h"

UCLASS()
class KIMBALLFOOD_API UKimballFoodDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Food")
    TSubclassOf<AActor> FoodActorClass;
};
