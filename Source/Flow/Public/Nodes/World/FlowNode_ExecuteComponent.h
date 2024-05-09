// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Types/FlowActorOwnerComponentRef.h"

#include "Nodes/FlowNode.h"

#include "FlowNode_ExecuteComponent.generated.h"

// Forward Declarations
class IFlowOwnerInterface;

/**
 * Execute a UActorComponent on the owning actor as if it was a flow subgraph
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Execute Component"))
class FLOW_API UFlowNode_ExecuteComponent : public UFlowNode
{
	GENERATED_BODY()

public:

	UFlowNode_ExecuteComponent();

	// IFlowCoreExecutableInterface
	virtual void InitializeInstance() override;
	virtual void DeinitializeInstance() override;
	virtual void PreloadContent() override;
	virtual void FlushContent() override;
	virtual void OnActivate() override;
	virtual void Cleanup() override;
	virtual void ForceFinishNode() override;
	virtual void ExecuteInput(const FName& PinName) override;
	// --

#if WITH_EDITOR
	// UObject
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	// --

	// UFlowNode
	virtual FText GetNodeTitle() const override;
	virtual EDataValidationResult ValidateNode() override;

	virtual FString GetStatusString() const override;
	// --
#endif // WITH_EDITOR

protected:

#if WITH_EDITOR
	void RefreshPins();
	const UActorComponent* TryGetExpectedComponent() const;
#endif // WITH_EDITOR

	UActorComponent* TryResolveComponent();
	TSubclassOf<AActor> TryGetExpectedActorOwnerClass() const;

protected:

	// Executable Component (by name) on the expected Flow owning Actor
	//  (the component must implement the IFlowExecutableComponentInterface)
	UPROPERTY(EditAnywhere, Category = "Flow Executable Component", meta = (DisplayName = "Component to Execute", MustImplement = "FlowCoreExecutableInterface,FlowExternalExecutableInterface", EditCondition = "!bIsInjectedComponent"))
	FFlowActorOwnerComponentRef ComponentRef;

	// Alternative, optional, injected component.  
	// If not found, it will be silently skipped by triggering the 1st output pin.
	UPROPERTY(EditAnywhere, Category = "Flow Executable Component", meta = (DisplayName = "Optional Injected Component", EditCondition = "bIsInjectedComponent"))
	FName InjectedComponentName;

	// Enable specifying the component by manually-entered name to be dynamically
	// linked at runtime to a dynamically injected component.
	UPROPERTY(EditAnywhere, Category = "Flow Executable Component");
	bool bIsInjectedComponent;
};
