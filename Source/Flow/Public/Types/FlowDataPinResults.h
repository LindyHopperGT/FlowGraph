// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Types/FlowPinEnums.h"

#include "GameplayTagContainer.h"

#include "FlowDataPinResults.generated.h"

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result")
struct FFlowDataPinResult
{
	GENERATED_BODY()

public:

	// Result for the DataPin resolve attempt
	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	EFlowDataPinResolveResult Result = EFlowDataPinResolveResult::FailedUnimplemented;

public:
	FFlowDataPinResult() { }
	FFlowDataPinResult(EFlowDataPinResolveResult InResult) : Result(InResult) { }
};

// Recommend implementing FFlowDataPinResult... for every EFlowPinType
FLOW_ASSERT_ENUM_MAX(EFlowPinType, 12);

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (Bool)")
struct FFlowDataPinResult_Bool : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	bool Value = false;

public:

	FFlowDataPinResult_Bool() { }
	FFlowDataPinResult_Bool(bool InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		{ }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (Int)")
struct FFlowDataPinResult_Int : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	int64 Value = 0;

public:

	FFlowDataPinResult_Int() { }
	FFlowDataPinResult_Int(int64 InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		{ }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (Float)")
struct FFlowDataPinResult_Float : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	double Value = 0;

public:

	FFlowDataPinResult_Float() { }
	FFlowDataPinResult_Float(double InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
	{ }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (Name)")
struct FFlowDataPinResult_Name : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	FName Value = NAME_None;

public:

	FFlowDataPinResult_Name() { }
	FFlowDataPinResult_Name(const FName& InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		{ }

	void SetValue(const FName& FromName) { Value = FromName; }
	void SetValue(const FString& FromString) { Value = FName(FromString); }
	void SetValue(const FText& FromText) { Value = FName(FromText.ToString()); }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (String)")
struct FFlowDataPinResult_String : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	FString Value;

public:

	FFlowDataPinResult_String() { }
	FFlowDataPinResult_String(const FString& InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		{ }

	void SetValue(const FName& FromName) { Value = FromName.ToString(); }
	void SetValue(const FString& FromString) { Value = FromString; }
	void SetValue(const FText& FromText) { Value = FromText.ToString(); }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (Text)")
struct FFlowDataPinResult_Text : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	FText Value;

public:

	FFlowDataPinResult_Text() { }
	FFlowDataPinResult_Text(const FText& InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		{ }

	void SetValue(const FName& FromName) { Value = FText::FromName(FromName); }
	void SetValue(const FString& FromString) { Value = FText::FromString(FromString); }
	void SetValue(const FText& FromText) { Value = FromText; }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (Enum)")
struct FFlowDataPinResult_Enum : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	// The selected enum Value
	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	FName Value = NAME_None;

	// Class for this enum
	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	UEnum* EnumClass = nullptr;

public:

	FFlowDataPinResult_Enum() { }
	FFlowDataPinResult_Enum(const FName& InValue, UEnum* InEnumClass)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		, EnumClass(InEnumClass)
		{ }

	template <typename TUnrealNativeEnumType>
	static FFlowDataPinResult_Enum BuildResultFromNativeEnumValue(TUnrealNativeEnumType EnumValue)
	{
		FFlowDataPinResult_Enum Result;
		Result.SetFromNativeEnumValue(EnumValue);

		return Result;
	}

	template <typename TUnrealNativeEnumType>
	void SetFromNativeEnumValue(TUnrealNativeEnumType InEnumValue)
	{
		EnumClass = StaticEnum<TUnrealNativeEnumType>();
		const FText DisplayValueText = EnumClass->GetDisplayValueAsText(InEnumValue);
		const FName EnumValue = FName(DisplayValueText.ToString());

		Value = EnumValue;
		Result = EFlowDataPinResolveResult::Success;
	}

	template <typename TUnrealNativeEnumType, TUnrealNativeEnumType InvalidValue>
	TUnrealNativeEnumType GetNativeEnumValue(EGetByNameFlags GetByNameFlags = EGetByNameFlags::None) const
	{
		if (!IsValid(EnumClass))
		{
			return InvalidValue;
		}

		int64 ValueAsInt = EnumClass->GetValueByName(Value, GetByNameFlags);
		if (ValueAsInt == INDEX_NONE)
		{
			return InvalidValue;
		}

		return static_cast<TUnrealNativeEnumType>(ValueAsInt);
	}
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (Vector)")
struct FFlowDataPinResult_Vector : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	FVector Value;

public:

	FFlowDataPinResult_Vector() { }
	FFlowDataPinResult_Vector(const FVector& InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		{ }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (Transform)")
struct FFlowDataPinResult_Transform : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	FTransform Value;

public:

	FFlowDataPinResult_Transform() { }
	FFlowDataPinResult_Transform(const FTransform& InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
	{ }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (GameplayTag)")
struct FFlowDataPinResult_GameplayTag : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	FGameplayTag Value;

public:

	FFlowDataPinResult_GameplayTag() { }
	FFlowDataPinResult_GameplayTag(const FGameplayTag& InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		{ }
};

USTRUCT(BlueprintType, DisplayName = "Flow DataPin Result (GameplayTagContainer)")
struct FFlowDataPinResult_GameplayTagContainer : public FFlowDataPinResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = DataPins)
	FGameplayTagContainer Value;

public:

	FFlowDataPinResult_GameplayTagContainer() { }
	FFlowDataPinResult_GameplayTagContainer(const FGameplayTagContainer& InValue)
		: Super(EFlowDataPinResolveResult::Success)
		, Value(InValue)
		{ }
};
