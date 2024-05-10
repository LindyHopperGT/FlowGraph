// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "UObject/Interface.h"
#include "Nodes/FlowPin.h"

#include "FlowContextPinSupplierInterface.generated.h"

// A flow element (UFlowNode, UFlowNodeAddOn, etc.) that may supply context pins
// "Context Pins" are those that can be dynamically added/removed to a FlowNode by property
// settings on the flow node, by subobjects, etc.
UINTERFACE(MinimalAPI, Blueprintable, DisplayName = "Flow ContextPin Supplier Interface")
class UFlowContextPinSupplierInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOW_API IFlowContextPinSupplierInterface
{
	GENERATED_BODY()

public:

	virtual bool SupportsContextPins() const { return true; }

#if WITH_EDITOR
	// Be careful, enabling it might cause loading gigabytes of data as nodes would load all related data (i.e. Level Sequences)
	virtual bool CanRefreshContextPinsOnLoad() const { return false; }
#endif // WITH_EDITOR

	UFUNCTION(BlueprintNativeEvent, Category = "FlowNode In-Editor Functions", DisplayName = "GetContextInputs", meta = (DevelopmentOnly))
	TArray<FFlowPin> K2_GetContextInputs() const;
	virtual TArray<FFlowPin> K2_GetContextInputs_Implementation() const;
	virtual TArray<FFlowPin> GetContextInputs() const { return Execute_K2_GetContextInputs(Cast<UObject>(this)); }

	UFUNCTION(BlueprintNativeEvent, Category = "FlowNode In-Editor Functions", DisplayName = "GetContextOutputs", meta = (DevelopmentOnly))
	TArray<FFlowPin> K2_GetContextOutputs() const;
	virtual TArray<FFlowPin> K2_GetContextOutputs_Implementation() const;
	virtual TArray<FFlowPin> GetContextOutputs() const { return Execute_K2_GetContextOutputs(Cast<UObject>(this)); }
};
