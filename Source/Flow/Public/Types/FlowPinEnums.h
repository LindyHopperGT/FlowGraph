// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Types/FlowEnumUtils.h"

#include "FlowPinEnums.generated.h"

UENUM(BlueprintType)
enum class EFlowPinType : uint8
{
	// Execution pin
	Exec,

	// FFlowDataPinProperty_Bool, FBoolProperty
	Bool,

	// FByteProperty FInt16Property FIntProperty FInt64Property FUInt16Property FUInt32Property FUInt64Property
	Int,

	// FFloatProperty, FDoubleProperty
	Float,

	// FNameProperty
	Name,

	// FStringProperty
	String,

	// FTextProperty
	Text,

	// FEnumProperty, FByteProperty
	Enum,

	// FVector (FStructProperty)
	Vector,

	// FTransform (FStructProperty)
	Transform,

	// FGameplayTag (FStructProperty)
	GameplayTag,

	// FGameplayTagContainer (FStructProperty)
	GameplayTagContainer,

#if 0 // TODO (gtaylor) finish support for Class/Object types
	// FObjectProperty, FObjectPtrProperty, FWeakObjectProperty, FLazyObjectProperty
	Object,

	// FSoftObjectProperty
	SoftObject,

	// FClassProperty, FClassPtrProperty
	Class,

	// FSoftClassProperty
	SoftClass,
#endif

	Max UMETA(Hidden),
	Invalid UMETA(Hidden),
	Min = 0 UMETA(Hidden),
};
//FLOW_ENUM_RANGE_VALUES(EFlowPinType);
ENUM_RANGE_BY_FIRST_AND_LAST(EFlowPinType, static_cast<int64>(EFlowPinType::Min), static_cast<int64>(EFlowPinType::Max) - 1)
FLOW_ENUM_STATIC_CAST_MIN_AND_MAX(EFlowPinType, EFlowPinType::Min, EFlowPinType::Max)
FLOW_ENUM_STATIC_CAST_TO_INT(EFlowPinType)
FLOW_ENUM_RANGE_UTILITY_FUNCTIONS(EFlowPinType)

// Result enum for TryResolveDataPinAs...() functions
UENUM(BlueprintType)
enum class EFlowDataPinResolveResult : uint8
{
	// Pin resolved successfully
	Success,

	// The pin is not connected to another pin
	FailedUnconnected,

	// The pin name is unknown
	FailedUnknownPin,

	// The pin was requested as an unsupported type
	FailedMismatchedType,

	// The Flow Node or AddOn did not implement the necessary function to provide this value
	FailedUnimplemented,

	// Failed with an error message (see the error log)
	FailedWithError,

	Max UMETA(Hidden),
};
