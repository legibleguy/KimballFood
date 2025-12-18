
#include "KimballFoodSettings.h"

UKimballFoodSettings::UKimballFoodSettings()
{
}

const UKimballFoodSettings* UKimballFoodSettings::Get()
{
    return GetDefault<UKimballFoodSettings>();
}
