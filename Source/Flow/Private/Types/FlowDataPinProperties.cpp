// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Types/FlowDataPinProperties.h"

#define LOCTEXT_NAMESPACE "FlowDataPinProperties"

#if WITH_EDITOR
FFlowPin FFlowDataPinProperty::CreateFlowPin(const FName& PinName, const TInstancedStruct<FFlowDataPinProperty>& DataPinProperty)
{
	FFlowPin FlowPin;

	const FFlowDataPinProperty* Property = DataPinProperty.GetPtr<FFlowDataPinProperty>();
	if (!Property)
	{
		return FlowPin;
	}

	FlowPin.PinName = PinName;

	const EFlowPinType FlowPinType = Property->GetFlowPinType();

	FLOW_ASSERT_ENUM_MAX(EFlowPinType, 13);

	switch (FlowPinType)
	{
	case EFlowPinType::Enum:
		{
			const FFlowDataPinOutputProperty_Enum& EnumDataPinProperty = DataPinProperty.Get<FFlowDataPinOutputProperty_Enum>();
			UEnum* EnumClass = EnumDataPinProperty.EnumClass;

			FlowPin.SetPinType(FlowPinType, EnumClass);
		}
		break;

#if 0
	case EFlowPinType::Object:
	case EFlowPinType::SoftObject:
	case EFlowPinType::Class:
	case EFlowPinType::SoftClass:
		{
			check(false);

			// TODO (gtaylor) We still need to implement Object, Class types
			FlowPin.SetPinType(FlowPinType);
		}
		break;
#endif // 0

	default:
		{
			FlowPin.SetPinType(FlowPinType);
		}
		break;
	}

	return FlowPin;
}

void FFlowDataPinOutputProperty_Enum::OnEnumNameChanged()
{
	if (!EnumName.IsEmpty())
	{
		EnumClass = UClass::TryFindTypeSlow<UEnum>(EnumName, EFindFirstObjectOptions::ExactClass);

		if (EnumClass != nullptr && !FFlowPin::ValidateEnum(*EnumClass))
		{
			EnumClass = nullptr;
		}
	}
}

FText FFlowNamedDataPinOutputProperty::BuildHeaderText() const
{
	EFlowPinType PinType = EFlowPinType::Invalid;

	if (const FFlowDataPinProperty* DataPinPropertyPtr = DataPinProperty.GetPtr<FFlowDataPinProperty>())
	{
		PinType = DataPinPropertyPtr->GetFlowPinType();
	}

	return FText::Format(LOCTEXT("FlowNamedDataPinOutputPropertyHeader", "{0} ({1})"), { FText::FromName(Name), UEnum::GetDisplayValueAsText(PinType) });
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
