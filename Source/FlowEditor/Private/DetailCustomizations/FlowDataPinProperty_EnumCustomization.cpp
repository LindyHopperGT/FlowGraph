// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "DetailCustomizations/FlowDataPinProperty_EnumCustomization.h"
#include "Types/FlowDataPinProperties.h"
#include "Nodes/FlowPin.h"

#include "IDetailChildrenBuilder.h"
#include "UObject/UnrealType.h"

// FFlowDataPinOutputProperty_EnumCustomization Implementation

void FFlowDataPinOutputProperty_EnumCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InStructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	if (TSharedPtr<IPropertyHandle> EnumClassHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FFlowDataPinOutputProperty_Enum, EnumClass)))
	{
		StructBuilder.AddProperty(EnumClassHandle.ToSharedRef());
	}

	if (TSharedPtr<IPropertyHandle> EnumNameHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FFlowDataPinOutputProperty_Enum, EnumName)))
	{
		StructBuilder.AddProperty(EnumNameHandle.ToSharedRef());

		EnumNameHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FFlowDataPinOutputProperty_EnumCustomization::OnEnumNameChanged));
	}
}

TSharedPtr<IPropertyHandle> FFlowDataPinOutputProperty_EnumCustomization::GetCuratedNamePropertyHandle() const
{
	check(StructPropertyHandle->IsValidHandle());

	TSharedPtr<IPropertyHandle> FoundHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FFlowDataPinOutputProperty_Enum, Value));
	check(FoundHandle);

	return FoundHandle;
}

TArray<FName> FFlowDataPinOutputProperty_EnumCustomization::GetCuratedNameOptions() const
{
	TArray<FName> Results;

	const UEnum* Enum = GetEnumClass();

	if (IsValid(Enum))
	{
		Results = GetEnumValues(*Enum);
	}

	return Results;
}

TArray<FName> FFlowDataPinOutputProperty_EnumCustomization::GetEnumValues(const UEnum& Enum)
{
	TArray<FName> EnumValues;

	for (int Index = 0; Index < Enum.GetMaxEnumValue(); Index++)
	{
		if (!Enum.IsValidEnumValue(Index))
		{
			continue;
		}

		static const TCHAR* MetaDataKey_Hidden = TEXT("Hidden");
		if (!Enum.HasMetaData(MetaDataKey_Hidden, Index))
		{
			EnumValues.Add(*Enum.GetDisplayNameTextByIndex(Index).ToString());
		}
	}

	return EnumValues;
}

void FFlowDataPinOutputProperty_EnumCustomization::SetCuratedName(const FName& NewValue)
{
	TSharedPtr<IPropertyHandle> ValueHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FFlowDataPinOutputProperty_Enum, Value));

	check(ValueHandle);

	ValueHandle->SetPerObjectValue(0, NewValue.ToString());
}

bool FFlowDataPinOutputProperty_EnumCustomization::TryGetCuratedName(FName& OutName) const
{
	if (const FFlowDataPinOutputProperty_Enum* ConfigurableEnumProperty = GetFlowDataPinEnumProperty())
	{
		OutName = ConfigurableEnumProperty->Value;

		return true;
	}
	else
	{
		return false;
	}
}

void FFlowDataPinOutputProperty_EnumCustomization::OnEnumNameChanged()
{
	if (FFlowDataPinOutputProperty_Enum* FlowDataPinEnumProperty = GetFlowDataPinEnumProperty())
	{
		FlowDataPinEnumProperty->OnEnumNameChanged();
	}
}

const UEnum* FFlowDataPinOutputProperty_EnumCustomization::GetEnumClass() const
{
	if (const FFlowDataPinOutputProperty_Enum* FlowDataPinEnumProperty = GetFlowDataPinEnumProperty())
	{
		return FlowDataPinEnumProperty->EnumClass;
	}

	return nullptr;
}
