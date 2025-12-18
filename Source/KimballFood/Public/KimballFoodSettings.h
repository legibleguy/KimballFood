
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "KimballFoodDataAsset.h"
#include "KimballFoodSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Kimball Food Settings")
class KIMBALLFOOD_API UKimballFoodSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UKimballFoodSettings();

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
    TSoftObjectPtr<UKimballFoodDataAsset> FoodDataAsset;

    static const UKimballFoodSettings* Get();
};
