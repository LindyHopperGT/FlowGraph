// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/DataPins/FlowNode_DefineProperties.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_DefineProperties)

UFlowNode_DefineProperties::UFlowNode_DefineProperties(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Data Pins");
	NodeStyle = EFlowNodeStyle::InOut;
#endif

	InputPins.Empty();
	OutputPins.Empty();

	AllowedSignalModes = { EFlowSignalMode::Enabled, EFlowSignalMode::Disabled };
}

bool UFlowNode_DefineProperties::TryFindPropertyByRemappedPinName(
	const FName& RemappedPinName,
	const FProperty*& OutFoundProperty,
	TInstancedStruct<FFlowDataPinProperty>& OutFoundInstancedStruct,
	EFlowDataPinResolveResult& InOutResult) const
{
	// The start node stores its properties in instanced structs in an array, so look there first

	for (const FFlowNamedDataPinOutputProperty& NamedProperty : OutputProperties)
	{
		if (NamedProperty.Name == RemappedPinName && NamedProperty.IsValid())
		{
			OutFoundInstancedStruct = NamedProperty.DataPinProperty;

			return true;
		}
	}

	return Super::TryFindPropertyByPinName(RemappedPinName, OutFoundProperty, OutFoundInstancedStruct, InOutResult);
}

#if WITH_EDITOR

void UFlowNode_DefineProperties::AutoGenerateDataPins(
	TMap<FName, FName>& InOutPinNameToBoundPropertyNameMap,
	TArray<FFlowPin>& InOutInputDataPins,
	TArray<FFlowPin>& InOutOutputDataPins) const
{
	for (const FFlowNamedDataPinOutputProperty& DataPinProperty : OutputProperties)
	{
		if (DataPinProperty.IsValid())
		{
			InOutPinNameToBoundPropertyNameMap.Add(DataPinProperty.Name, DataPinProperty.Name);

			InOutOutputDataPins.AddUnique(DataPinProperty.CreateFlowPin());
		}
	}
}

void UFlowNode_DefineProperties::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChainEvent)
{
	Super::PostEditChangeChainProperty(PropertyChainEvent);

	if (PropertyChainEvent.PropertyChain.Num() == 0)
	{
		return;
	}

	auto& Property = PropertyChainEvent.PropertyChain.GetActiveMemberNode()->GetValue();

	// The DetailsCustomization for FFlowDataPinProperties isn't being called when using an InstancedStruct
	// so we need to call this refresh by hand...
	if (PropertyChainEvent.ChangeType == EPropertyChangeType::ValueSet && 
		Property->GetFName() == GET_MEMBER_NAME_CHECKED(FFlowDataPinOutputProperty_Enum, EnumName))
	{
		for (FFlowNamedDataPinOutputProperty& OutputProperty : OutputProperties)
		{
			if (!OutputProperty.IsValid())
			{
				continue;
			}

			const FFlowDataPinProperty& FlowDataPinProperty = OutputProperty.DataPinProperty.Get();

			if (FlowDataPinProperty.GetFlowPinType() == EFlowPinType::Enum)
			{
				FFlowDataPinOutputProperty_Enum& EnumProperty = OutputProperty.DataPinProperty.GetMutable<FFlowDataPinOutputProperty_Enum>();
				EnumProperty.OnEnumNameChanged();
			}

			// We may need to manually call any PostEdit linked property updates here for future EFlowPinType values
			FLOW_ASSERT_ENUM_MAX(EFlowPinType, 16);
		}
	}

	constexpr EPropertyChangeType::Type RelevantChangeTypesForReconstructionMask =
		EPropertyChangeType::Unspecified |
		EPropertyChangeType::ArrayAdd |
		EPropertyChangeType::ArrayRemove |
		EPropertyChangeType::ArrayClear |
		EPropertyChangeType::ValueSet |
		EPropertyChangeType::Redirected |
		EPropertyChangeType::ArrayMove;

	const uint32 PropertyChangedTypeFlags = (PropertyChainEvent.ChangeType & RelevantChangeTypesForReconstructionMask);
	const bool bIsRelevantChangeTypeForReconstruction = PropertyChangedTypeFlags != 0;
	const bool bChangedOutputProperties = Property->GetFName() == GET_MEMBER_NAME_CHECKED(UFlowNode_DefineProperties, OutputProperties);
	if (bIsRelevantChangeTypeForReconstruction && bChangedOutputProperties)
	{
		OnReconstructionRequested.ExecuteIfBound();
	}
}
#endif // WITH_EDITOR
