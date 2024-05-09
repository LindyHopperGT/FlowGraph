// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Interfaces/FlowExternalExecutableInterface.h"
#include "Interfaces/FlowNativeExecutableInterface.h"

void IFlowExternalExecutableInterface::PreActivateExternalFlowExecutable(IFlowNativeExecutableInterface& NativeExecutorProxy)
{
	Execute_K2_PreActivateExternalFlowExecutable(Cast<UObject>(this), TScriptInterface<IFlowNativeExecutableInterface>(CastChecked<UObject>(&NativeExecutorProxy)));
}
