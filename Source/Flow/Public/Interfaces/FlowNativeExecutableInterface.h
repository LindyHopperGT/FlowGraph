// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "UObject/Interface.h"
#include "Nodes/FlowPin.h"

#include "FlowNativeExecutableInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, DisplayName = "[DEPRECATED] Flow Native Executable Interface", meta = (CannotImplementInterfaceInBlueprint))
class UFlowNativeExecutableInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOW_API IFlowNativeExecutableInterface
{
	GENERATED_BODY()

public:

	// NOTE (gtaylor) All of these functions have been moved into UFlowNodeBase.
	// Keeping the empty interface existing for a time until all of the assets are updated,
	// to prevent an assert.
};
