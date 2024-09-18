// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Types/FlowDataPinProperties.h"

#define LOCTEXT_NAMESPACE "FlowDataPinProperties"

FFlowPin FFlowNamedDataPinOutputProperty::CreateFlowPin() const
{
	FFlowPin FlowPin;

	const FFlowDataPinProperty* Property = DataPinProperty.GetPtr<FFlowDataPinProperty>();
	if (!Property)
	{
		return FlowPin;
	}

	FlowPin.PinName = Name;

	const EFlowPinType FlowPinType = Property->GetFlowPinType();

	FLOW_ASSERT_ENUM_MAX(EFlowPinType, 12);

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

#if WITH_EDITOR
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
	const FFlowDataPinProperty& DataPinPropertyRef = DataPinProperty.Get<FFlowDataPinProperty>();
	const EFlowPinType PinType = DataPinPropertyRef.GetFlowPinType();

	return FText::Format(LOCTEXT("FlowNamedDataPinOutputPropertyHeader", "{0} ({1})"), { FText::FromName(Name), UEnum::GetDisplayValueAsText(PinType) });
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
