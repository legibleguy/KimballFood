#include "FoodActor.h"
#include "KimballFoodSubsystem.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "FoodPositioningComponent.h"
#include "KimballFoodIngredient.h"
#include "Engine/StaticMeshSocket.h"

AFoodActor::AFoodActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
}

void AFoodActor::BeginPlay()
{
	Super::BeginPlay();

	// let's get the subsystem and set up our instanced mesh components
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return;

	UKimballFoodSubsystem* FoodSubsystem = GameInstance->GetSubsystem<UKimballFoodSubsystem>();
	if (!FoodSubsystem) return;

	TArray<TSoftObjectPtr<UStaticMesh>> UniqueMeshes;
	FoodSubsystem->GetAllUniqueMeshes(UniqueMeshes);

	// for each unique mesh, we create a component to handle instances of it
	for (const TSoftObjectPtr<UStaticMesh>& MeshPtr : UniqueMeshes)
	{
		if (!MeshPtr.IsNull())
		{
			UInstancedStaticMeshComponent* ISMC = NewObject<UInstancedStaticMeshComponent>(this);
			ISMC->RegisterComponent();
			ISMC->SetCanEverAffectNavigation(false);
			ISMC->SetStaticMesh(MeshPtr.LoadSynchronous());
			ISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			MeshToComponentMap.Add(MeshPtr, ISMC);
		}
	}
}

void AFoodActor::AddIngredient(const FGameplayTag& IngredientTag)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return;

	UKimballFoodSubsystem* FoodSubsystem = GameInstance->GetSubsystem<UKimballFoodSubsystem>();
	if (!FoodSubsystem) return;

	UKimballFoodIngredient* IngredientData = FoodSubsystem->GetIngredientByTag(IngredientTag);
	if (!IngredientData) return;

	// check if this ingredient requires a specific positioning component
	TSoftClassPtr<UFoodPositioningComponent> PositioningClassPtr = FoodSubsystem->GetPositioningComponentClassForTag(IngredientTag);
	if (!PositioningClassPtr.IsNull())
	{
		UClass* PositioningClass = PositioningClassPtr.LoadSynchronous();
		if (PositioningClass)
		{
			if (!PositioningComponent)
			{
				// if we don't have a positioning component yet, add the one this ingredient requires
				PositioningComponent = NewObject<UFoodPositioningComponent>(this, PositioningClass);
				PositioningComponent->RegisterComponent();

				// Catch up the new positioning component with the ingredients that have already been added
				for (const FGameplayTag& ExistingIngredientTag : AddedIngredients)
				{
					if (UKimballFoodIngredient* ExistingIngredientData = FoodSubsystem->GetIngredientByTag(ExistingIngredientTag))
					{
						PositioningComponent->OnIngredientAdded(ExistingIngredientData);
					}
				}
			}
			else if (PositioningComponent->GetClass() != PositioningClass)
			{
				// this ingredient requires a different positioning component than the one we have, so we can't add it
				UE_LOG(LogTemp, Warning, TEXT("Ingredient %s requires a different positioning component than the one already on the food actor. Cannot add."), *IngredientTag.ToString());
				return;
			}
		}
	}

	// find the mesh for the tag
	TSoftObjectPtr<UStaticMesh> MeshPtr = FoodSubsystem->GetMeshForTag(IngredientTag);
	if (MeshPtr.IsNull()) return;

	// find the right component for this mesh
	if (TObjectPtr<UInstancedStaticMeshComponent>* ISMC = MeshToComponentMap.Find(MeshPtr))
	{
		AddedIngredients.Add(IngredientTag);
		OnIngredientAdded.Broadcast(IngredientTag);

		if (PositioningComponent)
		{
			// if we have a positioning component, let it handle the placement
			PositioningComponent->OnIngredientAdded(IngredientData);
		}
		else
		{
			// otherwise, just stack 'em up.
			UStaticMesh* CurrentMesh = MeshPtr.Get();
			if (!CurrentMesh) return;

			FTransform NewInstanceTransform;
			bool bSocketFound = false;

			if (LastIngredientMesh)
			{
				UStaticMeshSocket* Socket = LastIngredientMesh->FindSocket(IngredientTag.GetTagName());
				if (Socket)
				{
					FTransform SocketTransform(Socket->RelativeRotation, Socket->RelativeLocation, Socket->RelativeScale);
					NewInstanceTransform = SocketTransform * LastInstanceTransform;
					bSocketFound = true;
				}
			}

			if (!bSocketFound)
			{
				NewInstanceTransform = FTransform(FVector(0, 0, CurrentStackHeight));
			}

			(*ISMC)->AddInstance(NewInstanceTransform);
			LastInstanceTransform = NewInstanceTransform;
			LastIngredientMesh = CurrentMesh;

			if (!bSocketFound)
			{
				CurrentStackHeight += CurrentMesh->GetBounds().BoxExtent.Z * 2;
			}
			else
			{
				CurrentStackHeight = NewInstanceTransform.GetLocation().Z + CurrentMesh->GetBounds().BoxExtent.Z * 2;
			}
		}
	}
}

void AFoodActor::ClearFood()
{
	// clear all instances from all our components
	for (auto& Elem : MeshToComponentMap)
	{
		if (Elem.Value)
		{
			Elem.Value->ClearInstances();
		}
	}

	// and reset our state
	AddedIngredients.Empty();
	CurrentStackHeight = 0.0f;
	LastIngredientMesh = nullptr;
	LastInstanceTransform = FTransform::Identity;

	if (PositioningComponent)
	{
		PositioningComponent->DestroyComponent();
		PositioningComponent = nullptr;
	}
}