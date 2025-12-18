#include "KimballFoodSubsystem.h"
#include "KimballFoodIngredient.h"
#include "Engine/AssetManager.h"
#include "GameplayTagsManager.h"
#include "Containers/Set.h"
#include "FoodPositioningComponent.h"

void UKimballFoodSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAssetManager& AssetManager = UAssetManager::Get();

	TArray<FAssetData> AssetDataList;
	AssetManager.GetPrimaryAssetDataList(TEXT("KimballFoodIngredient"), AssetDataList);

	for (const FAssetData& AssetData : AssetDataList)
	{
		auto FoodIngredient = Cast<UKimballFoodIngredient>(AssetData.GetAsset());
		if (!FoodIngredient) continue;
		
		FGameplayTag IngredientTag = FoodIngredient->IngredientTag;

		if (IngredientTag.IsValid())
		{
			IngredientDataMap.Add(IngredientTag, FoodIngredient);
		}

		TSoftObjectPtr<UStaticMesh> MeshPointer = FoodIngredient->Mesh;

		if (IngredientTag.IsValid() && MeshPointer.LoadSynchronous())
		{
			IngredientMeshMap.Add(IngredientTag, MeshPointer);
		}

		TSoftClassPtr<UFoodPositioningComponent> PositioningComponentClass = FoodIngredient->PositioningComponentClass;
		if (IngredientTag.IsValid() && !PositioningComponentClass.IsNull())
		{
			IngredientPositioningComponentMap.Add(IngredientTag, PositioningComponentClass);
		}
	}
}

void UKimballFoodSubsystem::Deinitialize()
{
	IngredientDataMap.Empty();
	IngredientMeshMap.Empty();
	IngredientPositioningComponentMap.Empty();
	Super::Deinitialize();
}

UKimballFoodIngredient* UKimballFoodSubsystem::GetIngredientByTag(const FGameplayTag& IngredientTag)
{
	return IngredientDataMap.FindRef(IngredientTag);
}

TSoftObjectPtr<UStaticMesh> UKimballFoodSubsystem::GetMeshForTag(const FGameplayTag& IngredientTag)
{
	return IngredientMeshMap.FindRef(IngredientTag);
}

TSoftClassPtr<UFoodPositioningComponent> UKimballFoodSubsystem::GetPositioningComponentClassForTag(const FGameplayTag& IngredientTag)
{
	return IngredientPositioningComponentMap.FindRef(IngredientTag);
}

void UKimballFoodSubsystem::GetAllUniqueMeshes(TArray<TSoftObjectPtr<UStaticMesh>>& OutMeshes)
{
	IngredientMeshMap.GenerateValueArray(OutMeshes);

	// remove any invalid pointers first
	OutMeshes.RemoveAll([](const TSoftObjectPtr<UStaticMesh>& MeshItem) {
		return !MeshItem.IsValid();
	});

	// use a TSet to easily get unique values
	TSet<TSoftObjectPtr<UStaticMesh>> UniqueMeshSet(OutMeshes);
	OutMeshes = UniqueMeshSet.Array();

	// sort the final array to ensure a consistent order for other systems
	OutMeshes.Sort([](const TSoftObjectPtr<UStaticMesh>& A, const TSoftObjectPtr<UStaticMesh>& B) {
		return A.ToString() < B.ToString();
	});
}