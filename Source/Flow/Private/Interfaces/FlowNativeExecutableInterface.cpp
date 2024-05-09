// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Interfaces/FlowNativeExecutableInterface.h"

void IFlowNativeExecutableInterface::TriggerOutputPin(const FFlowOutputPinHandle Pin, const bool bFinish, const EFlowPinActivationType ActivationType) 
{
	TriggerOutput(Pin.PinName, bFinish, ActivationType);
}

void IFlowNativeExecutableInterface::TriggerOutput(const FString& PinName, const bool bFinish)
{
	TriggerOutput(FName(PinName), bFinish);
}

void IFlowNativeExecutableInterface::TriggerOutput(const FText& PinName, const bool bFinish)
{
	TriggerOutput(FName(PinName.ToString()), bFinish);
}

void IFlowNativeExecutableInterface::TriggerOutput(const TCHAR* PinName, const bool bFinish)
{
	TriggerOutput(FName(PinName), bFinish);
}

