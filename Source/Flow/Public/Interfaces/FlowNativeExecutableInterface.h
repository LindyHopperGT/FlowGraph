// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "UObject/Interface.h"
#include "Nodes/FlowPin.h"

#include "FlowNativeExecutableInterface.generated.h"

// Implemented only by native (C++) objects that can execute within a flow graph
// (eg, UFlowNode and UFlowNodeAddOn subclasses implement this)
UINTERFACE(MinimalAPI, BlueprintType, DisplayName = "Flow Native Executable Interface", meta = (CannotImplementInterfaceInBlueprint))
class UFlowNativeExecutableInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOW_API IFlowNativeExecutableInterface
{
	GENERATED_BODY()

public:

	// Simply trigger the first Output Pin, convenient to use if node has only one output
	UFUNCTION(BlueprintCallable, Category = "FlowNode")
	virtual void TriggerFirstOutput(const bool bFinish) = 0;

	// Cause a specific output to be triggered (by PinName)
	UFUNCTION(BlueprintCallable, Category = "FlowNode", meta = (HidePin = "ActivationType"))
	virtual void TriggerOutput(const FName PinName, const bool bFinish = false, const EFlowPinActivationType ActivationType = EFlowPinActivationType::Default) = 0;

	// Cause a specific output to be triggered (by PinHandle)
	UFUNCTION(BlueprintCallable, Category = "FlowNode", meta = (HidePin = "ActivationType"))
	virtual void TriggerOutputPin(const FFlowOutputPinHandle Pin, const bool bFinish = false, const EFlowPinActivationType ActivationType = EFlowPinActivationType::Default);

	// TriggerOutput convenience aliases
	void TriggerOutput(const FString& PinName, const bool bFinish = false);
	void TriggerOutput(const FText& PinName, const bool bFinish = false);
	void TriggerOutput(const TCHAR* PinName, const bool bFinish = false);

	// Finish execution of node, it will call Cleanup
	UFUNCTION(BlueprintCallable, Category = "FlowNode")
	virtual void Finish() = 0;
};
