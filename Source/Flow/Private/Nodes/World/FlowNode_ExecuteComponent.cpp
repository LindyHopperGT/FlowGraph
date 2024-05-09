// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/World/FlowNode_ExecuteComponent.h"
#include "Interfaces/FlowOwnerInterface.h"
#include "Interfaces/FlowCoreExecutableInterface.h"
#include "Interfaces/FlowExternalExecutableInterface.h"
#include "Interfaces/FlowContextPinSupplierInterface.h"
#include "FlowAsset.h"
#include "FlowLogChannels.h"
#include "FlowSettings.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

#define LOCTEXT_NAMESPACE "FlowNode"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_ExecuteComponent)

UFlowNode_ExecuteComponent::UFlowNode_ExecuteComponent()
	: Super()
{
#if WITH_EDITOR
	NodeStyle = EFlowNodeStyle::Default;
	Category = TEXT("World");
#endif // WITH_EDITOR
}

void UFlowNode_ExecuteComponent::InitializeInstance()
{
	Super::InitializeInstance();

	if (UActorComponent* ResolvedComp = TryResolveComponent())
	{
		if (IFlowCoreExecutableInterface* ComponentAsCoreExecutable = Cast<IFlowCoreExecutableInterface>(ResolvedComp))
		{
			ComponentAsCoreExecutable->InitializeInstance();
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowCoreExecutableInterface::Execute_K2_InitializeInstance(ResolvedComp);
		}
	}
}

void UFlowNode_ExecuteComponent::DeinitializeInstance()
{
	if (UActorComponent* ResolvedComp = TryResolveComponent())
	{
		if (IFlowCoreExecutableInterface* ComponentAsCoreExecutable = Cast<IFlowCoreExecutableInterface>(ResolvedComp))
		{
			ComponentAsCoreExecutable->DeinitializeInstance();
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowCoreExecutableInterface::Execute_K2_DeinitializeInstance(ResolvedComp);
		}
	}

	Super::DeinitializeInstance();
}

void UFlowNode_ExecuteComponent::PreloadContent()
{
	Super::PreloadContent();

	if (UActorComponent* ResolvedComp = TryResolveComponent())
	{
		if (IFlowCoreExecutableInterface* ComponentAsCoreExecutable = Cast<IFlowCoreExecutableInterface>(ResolvedComp))
		{
			ComponentAsCoreExecutable->PreloadContent();
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowCoreExecutableInterface::Execute_K2_PreloadContent(ResolvedComp);
		}
	}
}

void UFlowNode_ExecuteComponent::FlushContent()
{
	if (UActorComponent* ResolvedComp = TryResolveComponent())
	{
		if (IFlowCoreExecutableInterface* ComponentAsCoreExecutable = Cast<IFlowCoreExecutableInterface>(ResolvedComp))
		{
			ComponentAsCoreExecutable->FlushContent();
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowCoreExecutableInterface::Execute_K2_FlushContent(ResolvedComp);
		}
	}

	Super::FlushContent();
}

void UFlowNode_ExecuteComponent::OnActivate()
{
	Super::OnActivate();

	if (UActorComponent* ResolvedComp = TryResolveComponent())
	{
		IFlowNativeExecutableInterface* ThisAsNativeExecutorProxy = CastChecked<IFlowNativeExecutableInterface>(this);

		if (IFlowExternalExecutableInterface* ComponentAsExternalExecutable = Cast<IFlowExternalExecutableInterface>(ResolvedComp))
		{
			// By convention, we must call the PreActivateExternalFlowExecutable() before OnActivate 
			// when we (this node) are acting as the proxy for an IFlowExternalExecutableInterface object
			ComponentAsExternalExecutable->PreActivateExternalFlowExecutable(*ThisAsNativeExecutorProxy);
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowExternalExecutableInterface::Execute_K2_PreActivateExternalFlowExecutable(ResolvedComp, TScriptInterface<IFlowNativeExecutableInterface>(this));
		}
		else
		{
			UE_LOG(LogFlow, Error, TEXT("Expected a valid UActorComponent that implemented the IFlowExternalExecutableInterface"));
		}

		if (IFlowCoreExecutableInterface* ComponentAsCoreExecutable = Cast<IFlowCoreExecutableInterface>(ResolvedComp))
		{
			ComponentAsCoreExecutable->OnActivate();
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowCoreExecutableInterface::Execute_K2_OnActivate(ResolvedComp);
		}
		else
		{
			UE_LOG(LogFlow, Error, TEXT("Expected a valid UActorComponent that implemented the IFlowCoreExecutableInterface"));
		}
	}
}

void UFlowNode_ExecuteComponent::Cleanup()
{
	if (UActorComponent* ResolvedComp = TryResolveComponent())
	{
		if (IFlowCoreExecutableInterface* ComponentAsCoreExecutable = Cast<IFlowCoreExecutableInterface>(ResolvedComp))
		{
			ComponentAsCoreExecutable->Cleanup();
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowCoreExecutableInterface::Execute_K2_Cleanup(ResolvedComp);
		}
	}

	Super::Cleanup();
}

void UFlowNode_ExecuteComponent::ForceFinishNode()
{
	if (UActorComponent* ResolvedComp = TryResolveComponent())
	{
		if (IFlowCoreExecutableInterface* ComponentAsCoreExecutable = Cast<IFlowCoreExecutableInterface>(ResolvedComp))
		{
			ComponentAsCoreExecutable->ForceFinishNode();
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowCoreExecutableInterface::Execute_K2_ForceFinishNode(ResolvedComp);
		}
	}

	Super::ForceFinishNode();
}

void UFlowNode_ExecuteComponent::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);

	if (UActorComponent* ResolvedComp = TryResolveComponent())
	{
		if (IFlowCoreExecutableInterface* ComponentAsCoreExecutable = Cast<IFlowCoreExecutableInterface>(ResolvedComp))
		{
			ComponentAsCoreExecutable->ExecuteInput(PinName);
		}
		else if (ResolvedComp->Implements<UFlowCoreExecutableInterface>())
		{
			IFlowCoreExecutableInterface::Execute_K2_ExecuteInput(ResolvedComp, PinName);
		}
	}
	else if (bIsInjectedComponent && !ComponentRef.GetResolvedComponent())
	{
		// If unresolved Injected component, pass through the node to the first output
		constexpr bool bFinish = true;
		TriggerFirstOutput(bFinish);

		UE_LOG(LogFlow, Verbose, TEXT("Injected component %s was skipped because it did not resolve"), *InjectedComponentName.ToString());
	}
}

UActorComponent* UFlowNode_ExecuteComponent::TryResolveComponent()
{
	UActorComponent* ResolvedComp = ComponentRef.GetResolvedComponent();
	if (IsValid(ResolvedComp))
	{
		return ResolvedComp;
	}

	AActor* ActorOwner = TryGetRootFlowActorOwner();

	if (!IsValid(ActorOwner))
	{
		UE_LOG(LogFlow, Error, TEXT("Expected a valid Actor owner to resolve component reference %s"), *ComponentRef.ComponentName.ToString());

		return nullptr;
	}

	if (bIsInjectedComponent)
	{
		// Apply the Injected name before trying to resolve 
		// (for components that were not available at authoring-time)
		ComponentRef.ComponentName = InjectedComponentName;
	}

	// Injected components are totally optional, if they are not there, we have to assume that's intentional.
	const bool bAllowWarnIfFailed = !bIsInjectedComponent;

	ResolvedComp = ComponentRef.TryResolveComponent(*ActorOwner, bAllowWarnIfFailed);

	return ResolvedComp;
}

#if WITH_EDITOR
const UActorComponent* UFlowNode_ExecuteComponent::TryGetExpectedComponent() const
{
	TSubclassOf<AActor> ExpectedOwnerClass = TryGetExpectedActorOwnerClass();
	if (bIsInjectedComponent)
	{
		// We might get lucky if the injected component is accessible... 
		// (but since it's an injected component, it probably won't be findable due to plugin rules)
		const UActorComponent* ExpectedComponent = AActor::GetActorClassDefaultComponentByName(ExpectedOwnerClass, InjectedComponentName);
		return ExpectedComponent;
	}
	else
	{
		const UActorComponent* ExpectedComponent = AActor::GetActorClassDefaultComponentByName(ExpectedOwnerClass, ComponentRef.ComponentName);
		return ExpectedComponent;
	}
}

void UFlowNode_ExecuteComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(FFlowActorOwnerComponentRef, ComponentName) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UFlowNode_ExecuteComponent, InjectedComponentName) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UFlowNode_ExecuteComponent, bIsInjectedComponent))
	{
		RefreshPins();
	}
}

void UFlowNode_ExecuteComponent::RefreshPins()
{
	bool bChangedPins = false;

	const UActorComponent* ExpectedComponent = TryGetExpectedComponent();
	if (const IFlowContextPinSupplierInterface* ContextPinSupplierInterface = Cast<IFlowContextPinSupplierInterface>(ExpectedComponent))
	{
		const TArray<FFlowPin> NewInputPins = ContextPinSupplierInterface->GetContextInputs();
		bChangedPins = RebuildPinArray(NewInputPins, InputPins, DefaultInputPin) || bChangedPins;

		const TArray<FFlowPin> NewOutputPins = ContextPinSupplierInterface->GetContextOutputs();
		bChangedPins = RebuildPinArray(NewOutputPins, OutputPins, DefaultOutputPin) || bChangedPins;
	}
	else
	{
		bChangedPins = RebuildPinArray(TArray<FName>(&DefaultInputPin.PinName, 1), InputPins, DefaultInputPin) || bChangedPins;
		bChangedPins = RebuildPinArray(TArray<FName>(&DefaultOutputPin.PinName, 1), OutputPins, DefaultOutputPin) || bChangedPins;
	}

	if (bChangedPins)
	{
		OnReconstructionRequested.ExecuteIfBound();
	}
}

EDataValidationResult UFlowNode_ExecuteComponent::ValidateNode()
{
	const bool bHasComponent = ComponentRef.IsConfigured();
	if (!bHasComponent)
	{
		ValidationLog.Error<UFlowNode>(TEXT("ExectuteComponent requires a valid Compoennt reference"), this);

		return EDataValidationResult::Invalid;
	}

	TSubclassOf<AActor> ExpectedActorOwnerClass = TryGetExpectedActorOwnerClass();
	if (!IsValid(ExpectedActorOwnerClass))
	{
		ValidationLog.Error<UFlowNode>(TEXT("Invalid or null Expected Actor Owner Class for this Flow Asset"), this);

		return EDataValidationResult::Invalid;
	}

	if (bIsInjectedComponent)
	{
		// Not much validation we can do for injected components but hope that they're there at runtime.
		if (!InjectedComponentName.IsValid())
		{
			ValidationLog.Error<UFlowNode>(TEXT("Must have a valid Injected Component Name"), this);

			return EDataValidationResult::Invalid;
		}
	}
	else
	{
		// Check if the component can be found on the expected owner
		const UActorComponent* ExpectedComponent = TryGetExpectedComponent();
		if (!IsValid(ExpectedComponent))
		{
			ValidationLog.Error<UFlowNode>(TEXT("Could not resolve component for flow actor owner"), this);

			return EDataValidationResult::Invalid;
		}

		// Check that the component implements the expected interfaces
		if (!Cast<IFlowExternalExecutableInterface>(ExpectedComponent))
		{
			ValidationLog.Error<UFlowNode>(TEXT("Expected component to implement IFlowExternalExecutableInterface"), this);

			return EDataValidationResult::Invalid;
		}

		if (!Cast<IFlowCoreExecutableInterface>(ExpectedComponent))
		{
			ValidationLog.Error<UFlowNode>(TEXT("Expected component to implement IFlowCoreExecutableInterface"), this);

			return EDataValidationResult::Invalid;
		}
	}

	return EDataValidationResult::Valid;
}

FString UFlowNode_ExecuteComponent::GetStatusString() const
{
	if (ActivationState != EFlowNodeState::NeverActivated)
	{
		return UEnum::GetDisplayValueAsText(ActivationState).ToString();
	}

	return Super::GetStatusString();
}

TSubclassOf<AActor> UFlowNode_ExecuteComponent::TryGetExpectedActorOwnerClass() const
{
	const UFlowAsset* FlowAsset = GetFlowAsset();
	if (IsValid(FlowAsset))
	{
		return FlowAsset->GetExpectedOwnerClass();
	}

	return nullptr;
}

FText UFlowNode_ExecuteComponent::GetNodeTitle() const
{
	const bool bUseAdaptiveNodeTitles = UFlowSettings::Get()->bUseAdaptiveNodeTitles;

	if (bUseAdaptiveNodeTitles)
	{
		if (!bIsInjectedComponent && !ComponentRef.ComponentName.IsNone())
		{
			const FText ComponentNameText = FText::FromName(ComponentRef.ComponentName);

			return FText::Format(LOCTEXT("ExecuteComponent", "Execute {0}"), { ComponentNameText });
		}
		
		if (bIsInjectedComponent && InjectedComponentName.IsValid())
		{
			const FText ComponentNameText = FText::FromName(InjectedComponentName);

			return FText::Format(LOCTEXT("ExecuteComponent", "Execute Injected {0}"), { ComponentNameText });
		}
	}

	return Super::GetNodeTitle();
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
