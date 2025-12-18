
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "KimballFoodCheatManager.generated.h"

/**
 * cheat manager for all your food-related testing needs
 */
UCLASS()
class KIMBALLFOOD_API UKimballFoodCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	// spawns a food actor right at the world origin
	UFUNCTION(Exec, BlueprintCallable)
	void Cheat_SpawnFoodActorAtOrigin();

	// spawns a food actor a certain distance in front of the player camera
	UFUNCTION(Exec, BlueprintCallable)
	void Cheat_SpawnFoodActorInFront(float Distance = 100.0f);

	// adds an ingredient to the first food actor it can find
	UFUNCTION(Exec, BlueprintCallable)
	void Cheat_AddIngredient(const FString& TagString);

	// prints all the food entries that the subsystem finds
	UFUNCTION(Exec, BlueprintCallable)
	void Cheat_PrintAllFoodEntries();
};
