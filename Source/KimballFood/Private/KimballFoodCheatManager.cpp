
#include "KimballFoodCheatManager.h"
#include "FoodActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GameplayTagsManager.h"
#include "KimballFoodSubsystem.h"
#include "KimballFoodSettings.h"
#include "KimballFoodDataAsset.h"

void UKimballFoodCheatManager::Cheat_SpawnFoodActorAtOrigin()
{
	UWorld* World = GetWorld();
	if (!World) return;

	TSubclassOf<AActor> FoodActorClass = AFoodActor::StaticClass();
	if (const UKimballFoodSettings* FoodSettings = UKimballFoodSettings::Get())
	{
		if (const UKimballFoodDataAsset* FoodDataAsset = FoodSettings->FoodDataAsset.LoadSynchronous())
		{
			if (FoodDataAsset->FoodActorClass)
			{
				FoodActorClass = FoodDataAsset->FoodActorClass;
			}
		}
	}

	World->SpawnActor<AActor>(FoodActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
}

void UKimballFoodCheatManager::Cheat_SpawnFoodActorInFront(float Distance)
{
	UWorld* World = GetWorld();
	APlayerController* PC = GetOuterAPlayerController();
	if (!World || !PC) return;

	TSubclassOf<AActor> FoodActorClass = AFoodActor::StaticClass();
	if (const UKimballFoodSettings* FoodSettings = UKimballFoodSettings::Get())
	{
		if (const UKimballFoodDataAsset* FoodDataAsset = FoodSettings->FoodDataAsset.LoadSynchronous())
		{
			if (FoodDataAsset->FoodActorClass)
			{
				FoodActorClass = FoodDataAsset->FoodActorClass;
			}
		}
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector SpawnLocation = CameraLocation + (CameraRotation.Vector() * Distance);
	World->SpawnActor<AActor>(FoodActorClass, SpawnLocation, CameraRotation);
}

void UKimballFoodCheatManager::Cheat_AddIngredient(const FString& TagString)
{
	UWorld* World = GetWorld();
	if (!World) return;

	// find the first food actor in the world
	AFoodActor* FoodActor = nullptr;
	for (TActorIterator<AFoodActor> It(World); It; ++It)
	{
		FoodActor = *It;
		break;
	}

	if (FoodActor)
	{
		// convert the string to a gameplay tag
		FGameplayTag IngredientTag = UGameplayTagsManager::Get().RequestGameplayTag(*TagString);
		if (IngredientTag.IsValid())
		{
			// check if the tag is a valid ingredient
			UKimballFoodSubsystem* FoodSubsystem = World->GetGameInstance()->GetSubsystem<UKimballFoodSubsystem>();
			if (FoodSubsystem && FoodSubsystem->GetMeshForTag(IngredientTag) != nullptr)
			{
				FoodActor->AddIngredient(IngredientTag);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("invalid or unknown ingredient tag: %s"), *TagString));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("invalid gameplay tag: %s"), *TagString));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("couldn't find any AFoodActor in the world"));
	}
}

void UKimballFoodCheatManager::Cheat_PrintAllFoodEntries()
{
	UWorld* World = GetWorld();
	if (!World) return;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find GameInstance."));
		return;
	}

	UKimballFoodSubsystem* FoodSubsystem = GameInstance->GetSubsystem<UKimballFoodSubsystem>();
	if (FoodSubsystem)
	{
		const TMap<FGameplayTag, TSoftObjectPtr<UStaticMesh>>& IngredientMeshMap = FoodSubsystem->GetIngredientMeshMap();

		if (IngredientMeshMap.Num() == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("No food entries found."));
			return;
		}

		for (const auto& Elem : IngredientMeshMap)
		{
			FString TagName = Elem.Key.ToString();
			FString MeshPath = Elem.Value.ToString();
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Tag: %s, Mesh: %s"), *TagName, *MeshPath));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find KimballFoodSubsystem."));
	}
}
