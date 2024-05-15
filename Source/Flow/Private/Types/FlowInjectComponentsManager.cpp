// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Types/FlowInjectComponentsManager.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "FlowLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowInjectComponentsManager)

void UFlowInjectComponentsManager::InitializeRuntime()
{
	check(ActorToComponentsMap.IsEmpty());
}

void UFlowInjectComponentsManager::ShutdownRuntime()
{
	if (bRemoveInjectedComponentsWhenDeinitializing)
	{
		RemoveInjectedComponents();
	}

	ActorToComponentsMap.Empty();
}

void UFlowInjectComponentsManager::InjectComponentsOnActor(AActor& Actor, const TArray<UActorComponent*>& ComponentInstances)
{
	for (UActorComponent* ComponentInstance : ComponentInstances)
	{
		if (IsValid(ComponentInstance))
		{
			AddAndRegisterComponent(Actor, *ComponentInstance);
		}
	}
}

void UFlowInjectComponentsManager::RemoveInjectedComponents()
{
	for (auto& KV : ActorToComponentsMap)
	{
		AActor* Actor = KV.Key;
		const FFlowComponentInstances& Instances = KV.Value;

		if (!IsValid(Actor))
		{
			continue;
		}

		for (TWeakObjectPtr<UActorComponent> ComponentInstancePtr : Instances.Components)
		{
			if (UActorComponent* ComponentInstance = ComponentInstancePtr.Get())
			{
				RemoveAndUnregisterComponent(*Actor, *ComponentInstance);
			}
		}
	}
}

void UFlowInjectComponentsManager::AddAndRegisterComponent(AActor& Actor, UActorComponent& ComponentInstance)
{
	// Following pattern from UGameFrameworkComponentManager::CreateComponentOnInstance()
	if (USceneComponent* SceneComponentInstance = Cast<USceneComponent>(&ComponentInstance))
	{
		SceneComponentInstance->SetupAttachment(Actor.GetRootComponent());
	}

	ComponentInstance.RegisterComponent();

	if (bRemoveInjectedComponentsWhenDeinitializing)
	{
		// If we will be responsible for removing them later,
		// we need to keep track of the spawned components
		FFlowComponentInstances& ComponentInstances = ActorToComponentsMap.FindOrAdd(&Actor);
		ComponentInstances.Components.Add(&ComponentInstance);

		RegisterOnDestroyedDelegate(Actor);
	}
}

void UFlowInjectComponentsManager::RemoveAndUnregisterComponent(AActor& Actor, UActorComponent& ComponentInstance)
{
	BeforeActorRemovedDelegate.Broadcast(&Actor);

	UnregisterOnDestroyedDelegate(Actor);

	// Following pattern from UGameFrameworkComponentManager::DestroyInstancedComponent()
	ComponentInstance.DestroyComponent();
	ComponentInstance.SetFlags(RF_Transient);
}

void UFlowInjectComponentsManager::RegisterOnDestroyedDelegate(AActor& Actor)
{
	Actor.OnDestroyed.AddDynamic(this, &UFlowInjectComponentsManager::OnActorDestroyed);
}

void UFlowInjectComponentsManager::UnregisterOnDestroyedDelegate(AActor& Actor)
{
	Actor.OnDestroyed.RemoveDynamic(this, &UFlowInjectComponentsManager::OnActorDestroyed);
}

void UFlowInjectComponentsManager::RemoveAllInjectedComponentsAndStopMonitoringActor(AActor& Actor)
{
	const FFlowComponentInstances* FoundComponentInstances = ActorToComponentsMap.Find(&Actor);

	if (ensure(FoundComponentInstances))
	{
		for (TWeakObjectPtr<UActorComponent> ComponentInstancePtr : FoundComponentInstances->Components)
		{
			if (UActorComponent* ComponentInstance = ComponentInstancePtr.Get())
			{
				RemoveAndUnregisterComponent(Actor, *ComponentInstance);
			}
		}
	}

	ActorToComponentsMap.Remove(&Actor);
}

void UFlowInjectComponentsManager::OnActorDestroyed(AActor* DestroyedActor)
{
	if (IsValid(DestroyedActor))
	{
		RemoveAllInjectedComponentsAndStopMonitoringActor(*DestroyedActor);
	}
}
