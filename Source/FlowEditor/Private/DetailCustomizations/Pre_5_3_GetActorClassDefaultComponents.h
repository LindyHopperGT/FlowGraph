// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Runtime/Launch/Resources/Version.h"

// NOTE (gtaylor) Compatibility patch for pre-5.3 UE to support AActor::GetActorClassDefaultComponents()
#define USE_PRE_5_3_GET_ACTOR_CLASS_DEFAULT_COMPONENTS (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 3)

#if USE_PRE_5_3_GET_ACTOR_CLASS_DEFAULT_COMPONENTS

#include "Components/ActorComponent.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "GameFramework/Actor.h"

FORCEINLINE_DEBUGGABLE void Pre_5_3_ForEachComponentOfActorClassDefault(const TSubclassOf<AActor>& ActorClass, const TSubclassOf<UActorComponent>& InComponentClass, TFunctionRef<bool(const UActorComponent*)> InFunc)
{
	// Adapted from UE 5.3 AActor::ForEachComponentOfActorClassDefault()

	if (!ActorClass.Get())
	{
		return;
	}

	auto FilterFunc = [&](const UActorComponent* TemplateComponent)
		{
			if (!TemplateComponent)
			{
				return true;
			}

			if (!InComponentClass.Get() || TemplateComponent->IsA(InComponentClass))
			{
				return InFunc(TemplateComponent);
			}

			return true;
		};

		// Process native components
	const AActor* CDO = ActorClass->GetDefaultObject<AActor>();
	for (const UActorComponent* Component : CDO->GetComponents())
	{
		if (!FilterFunc(Component))
		{
			return;
		}
	}

	// Process blueprint components
	if (UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass))
	{
		UBlueprintGeneratedClass::ForEachGeneratedClassInHierarchy(ActorClass, [&](const UBlueprintGeneratedClass* CurrentBPGC)
			{
				if (const USimpleConstructionScript* const ConstructionScript = CurrentBPGC->SimpleConstructionScript)
				{
					// Gets all BP added components
					for (const USCS_Node* const Node : ConstructionScript->GetAllNodes())
					{
						if (!FilterFunc(Node->GetActualComponentTemplate(ActorBlueprintGeneratedClass)))
						{
							return false;
						}
					}
				}
				return true;
			});
	}
}

template <typename TComponentClass = UActorComponent, typename = typename TEnableIf<TIsDerivedFrom<TComponentClass, UActorComponent>::IsDerived>::Type>
static void Pre_5_3_GetActorClassDefaultComponents(const TSubclassOf<AActor>& InActorClass, TArray<const TComponentClass*>& OutComponents)
{
	// Adapted from UE 5.3 AActor::GetActorClassDefaultComponents()

	Pre_5_3_ForEachComponentOfActorClassDefault(InActorClass, TComponentClass::StaticClass(), [&](const UActorComponent* TemplateComponent)
		{
			OutComponents.Add(CastChecked<TComponentClass>(TemplateComponent));
			return true;
		});
}
#endif
