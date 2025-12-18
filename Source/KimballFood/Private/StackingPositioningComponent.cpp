#include "StackingPositioningComponent.h"
#include "KimballFoodIngredient.h"
#include "FoodActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"

UStackingPositioningComponent::UStackingPositioningComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStackingPositioningComponent::OnIngredientAdded(UKimballFoodIngredient* Ingredient)
{
	Super::OnIngredientAdded(Ingredient);

	if (!Ingredient) return;

	// categorize the ingredient and store it
	if (Ingredient->IngredientTag == BottomBunTag)
	{
		BottomBun = Ingredient;
	}
	else if (Ingredient->IngredientTag == TopBunTag)
	{
		TopBun = Ingredient;
	}
	else
	{
		MiddleIngredients.Add(Ingredient);
	}

	RebuildStack();
}

void UStackingPositioningComponent::RebuildStack()
{
	AFoodActor* FoodActor = GetOwner<AFoodActor>();
	if (!FoodActor) return;

	const auto& MeshMap = FoodActor->GetMeshToComponentMap();

	// clear all current instances
	for (const auto& Elem : MeshMap)
	{
		if (Elem.Value)
		{
			Elem.Value->ClearInstances();
		}
	}

	TArray<UKimballFoodIngredient*> AllIngredients;
	if (BottomBun)
	{
		AllIngredients.Add(BottomBun);
	}
	AllIngredients.Append(MiddleIngredients);
	if (TopBun)
	{
		AllIngredients.Add(TopBun);
	}

	if (AllIngredients.IsEmpty())
	{
		return;
	}

	FTransform LastInstanceTransform = FTransform::Identity;
	UStaticMesh* LastIngredientMesh = nullptr;
    float CurrentHeight = 0.f;

	for (int32 i = 0; i < AllIngredients.Num(); ++i)
	{
		UKimballFoodIngredient* CurrentIngredient = AllIngredients[i];
		if (!CurrentIngredient) continue;

		UStaticMesh* CurrentMesh = CurrentIngredient->Mesh.Get();
		if (!CurrentMesh) continue;

		if (const TObjectPtr<UInstancedStaticMeshComponent>* ISMC = MeshMap.Find(CurrentIngredient->Mesh))
		{
            FTransform NewInstanceTransform;
            bool bSocketFound = false;

            if (LastIngredientMesh)
            {
                UStaticMeshSocket* Socket = LastIngredientMesh->FindSocket(CurrentIngredient->IngredientTag.GetTagName());
                if (Socket)
                {
                    FTransform SocketTransform(Socket->RelativeRotation, Socket->RelativeLocation, Socket->RelativeScale);
                    NewInstanceTransform = SocketTransform * LastInstanceTransform;
                    bSocketFound = true;
                }
            }

            if (!bSocketFound)
            {
                NewInstanceTransform = FTransform(FVector(0, 0, CurrentHeight));
            }

			(*ISMC)->AddInstance(NewInstanceTransform);
			LastInstanceTransform = NewInstanceTransform;
			LastIngredientMesh = CurrentMesh;

            if (!bSocketFound)
            {
                CurrentHeight += CurrentMesh->GetBounds().BoxExtent.Z * 2;
            }
            else
            {
                // When a socket is used, the next non-socketed item should be placed on top of it.
                CurrentHeight = NewInstanceTransform.GetLocation().Z + CurrentMesh->GetBounds().BoxExtent.Z * 2;
            }
		}
	}
}