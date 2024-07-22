// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "UObject/ObjectMacros.h"
#include "FlowPin.generated.h"

USTRUCT(BlueprintType)
struct FLOW_API FFlowPin
{
	GENERATED_BODY()

	// A logical name, used during execution of pin
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlowPin")
	FName PinName;

	// An optional Display Name, you can use it to override PinName without the need to update graph connections
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlowPin")
	FText PinFriendlyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlowPin")
	FString PinToolTip;

	/** Category of pin type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlowPin")
	FName PinCategory = PinCategoryDefault;

	/** Sub-category object */
	UPROPERTY()
	TWeakObjectPtr<UObject> PinSubCategoryObject;

	// PinCategory aliases for those defined in UEdGraphSchema_K2
	static inline FName PC_Exec = TEXT("exec");
	static inline FName PC_Boolean = TEXT("bool");
	static inline FName PC_Byte = TEXT("byte");
	static inline FName PC_Class = TEXT("class");
	static inline FName PC_Int = TEXT("int");
	static inline FName PC_Int64 = TEXT("int64");
	static inline FName PC_Float = TEXT("float");
	static inline FName PC_Double = TEXT("double");
	static inline FName PC_Real = TEXT("real");
	static inline FName PC_Name = TEXT("name");
	static inline FName PC_Delegate = TEXT("delegate");
	static inline FName PC_MCDelegate = TEXT("mcdelegate");
	static inline FName PC_Object = TEXT("object");
	static inline FName PC_Interface = TEXT("interface");
	static inline FName PC_String = TEXT("string");
	static inline FName PC_Text = TEXT("text");
	static inline FName PC_Struct = TEXT("struct");
	static inline FName PC_Wildcard = TEXT("wildcard");
	static inline FName PC_FieldPath = TEXT("fieldpath");
	static inline FName PC_Enum = TEXT("enum");
	static inline FName PC_SoftObject = TEXT("softobject");
	static inline FName PC_SoftClass = TEXT("softclass");

	// Default pin type in Flow is the "Exec" pin type
	static inline FName PinCategoryDefault = PC_Exec;

	static inline FName AnyPinName = TEXT("AnyPinName");

	FFlowPin()
		: PinName(NAME_None)
	{
	}

	FFlowPin(const FName& InPinName)
		: PinName(InPinName)
	{
	}

	FFlowPin(const FString& InPinName)
		: PinName(*InPinName)
	{
	}

	FFlowPin(const FText& InPinName)
		: PinName(*InPinName.ToString())
	{
	}

	FFlowPin(const TCHAR* InPinName)
		: PinName(FName(InPinName))
	{
	}

	FFlowPin(const uint8& InPinName)
		: PinName(FName(*FString::FromInt(InPinName)))
	{
	}

	FFlowPin(const int32& InPinName)
		: PinName(FName(*FString::FromInt(InPinName)))
	{
	}

	FFlowPin(const FStringView InPinName, const FText& InPinFriendlyName)
		: PinName(InPinName)
		, PinFriendlyName(InPinFriendlyName)
	{
	}

	FFlowPin(const FStringView InPinName, const FString& InPinTooltip)
		: PinName(InPinName)
		, PinToolTip(InPinTooltip)
	{
	}

	FFlowPin(const FStringView InPinName, const FText& InPinFriendlyName, const FString& InPinTooltip)
		: PinName(InPinName)
		, PinFriendlyName(InPinFriendlyName)
		, PinToolTip(InPinTooltip)
	{
	}

	FORCEINLINE bool IsValid() const
	{
		return !PinName.IsNone();
	}

	FORCEINLINE bool operator==(const FFlowPin& Other) const
	{
		return PinName == Other.PinName;
	}

	FORCEINLINE bool operator!=(const FFlowPin& Other) const
	{
		return PinName != Other.PinName;
	}

	FORCEINLINE bool operator==(const FName& Other) const
	{
		return PinName == Other;
	}

	FORCEINLINE bool operator!=(const FName& Other) const
	{
		return PinName != Other;
	}

	friend uint32 GetTypeHash(const FFlowPin& FlowPin)
	{
		return GetTypeHash(FlowPin.PinName);
	}

	// FFlowPin instance signatures for "trait" functions
	FORCEINLINE bool IsExecPin() const { return IsExecPin(*this); }
	FORCEINLINE static bool IsExecPin(const FFlowPin& FlowPin) { return IsExecPinCategory(FlowPin.PinCategory); }
	//--

	// PinCategory "trait" functions:
	FORCEINLINE static bool IsExecPinCategory(const FName& PC) { return PC == PC_Exec; }
	FORCEINLINE static bool IsDataPinCategory(const FName& PC) { return PC != PC_Exec; }
	FORCEINLINE static bool IsIndexPinCategory(const FName& PC) { return IsConvertableToInt64PinCategory(PC); }
	FORCEINLINE static bool IsNumericPinCategory(const FName& PC) { return IsConvertableToInt64PinCategory(PC) || IsConvertableToDoublePinCategory(PC); }
	FORCEINLINE static bool IsTextBasedPinCategory(const FName& PC) { return PC == PC_Name || PC == PC_String || PC == PC_Text; }
	FORCEINLINE static bool IsDelegatePinCategory(const FName& PC) { return PC == PC_Delegate || PC == PC_MCDelegate; }
	FORCEINLINE static bool IsWildcardPinCategory(const FName& PC) { return PC == PC_Wildcard; }

	// This pin's data a composite type? (ie, struct or class)
	FORCEINLINE static bool IsCompositePinCategory(const FName& PC) { return PC == PC_Class || PC == PC_Object || PC == PC_Struct; }

	// Is the PinSubCategoryObject field valid for this pin?
	FORCEINLINE static bool IsSubtypeSupportedPinCategory(const FName& PC) { return IsCompositePinCategory(PC) || PC == PC_Interface; }

	// IsConvertable trait functions:
	FORCEINLINE static bool IsConvertableToDoublePinCategory(const FName& PC) { return PC == PC_Double || PC == PC_Float || PC == PC_Real; }
	FORCEINLINE static bool IsConvertableToInt64PinCategory(const FName& PC) { return PC == PC_Boolean || PC == PC_Byte || PC == PC_Int || PC == PC_Int64; }
	FORCEINLINE static bool IsConvertableToObjectPinCategory(const FName& PC) { return PC == PC_Object || PC == PC_SoftObject; }
	FORCEINLINE static bool IsConvertableToClassPinCategory(const FName& PC) { return PC == PC_Class || PC == PC_SoftClass; }
	FORCEINLINE static bool IsConvertableToBoolPinCategory(const FName& PC) 
		{ return IsConvertableToInt64PinCategory(PC) || IsConvertableToObjectPinCategory(PC) || IsConvertableToClassPinCategory(PC) ||
			     IsTextBasedPinCategory(PC) || IsDelegatePinCategory(PC); }
	FORCEINLINE static bool IsConvertableToTextPinCategory(const FName& PC) { return IsTextBasedPinCategory(PC) || PC == PC_Enum; }
	//--
};

USTRUCT()
struct FLOW_API FFlowPinHandle
{
	GENERATED_BODY()

	// Update SFlowPinHandleBase code if this property name would be ever changed
	UPROPERTY()
	FName PinName;

	FFlowPinHandle()
		: PinName(NAME_None)
	{
	}
};

USTRUCT(BlueprintType)
struct FLOW_API FFlowInputPinHandle : public FFlowPinHandle
{
	GENERATED_BODY()

	FFlowInputPinHandle()
	{
	}
};

USTRUCT(BlueprintType)
struct FLOW_API FFlowOutputPinHandle : public FFlowPinHandle
{
	GENERATED_BODY()

	FFlowOutputPinHandle()
	{
	}
};

// Processing Flow Nodes creates map of connected pins
USTRUCT()
struct FLOW_API FConnectedPin
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FGuid NodeGuid;

	UPROPERTY()
	FName PinName;

	FConnectedPin()
		: NodeGuid(FGuid())
		, PinName(NAME_None)
	{
	}

	FConnectedPin(const FGuid InNodeId, const FName& InPinName)
		: NodeGuid(InNodeId)
		, PinName(InPinName)
	{
	}

	FORCEINLINE bool operator==(const FConnectedPin& Other) const
	{
		return NodeGuid == Other.NodeGuid && PinName == Other.PinName;
	}

	FORCEINLINE bool operator!=(const FConnectedPin& Other) const
	{
		return NodeGuid != Other.NodeGuid || PinName != Other.PinName;
	}

	friend uint32 GetTypeHash(const FConnectedPin& ConnectedPin)
	{
		return GetTypeHash(ConnectedPin.NodeGuid) + GetTypeHash(ConnectedPin.PinName);
	}
};

UENUM(BlueprintType)
enum class EFlowPinActivationType : uint8
{
	Default,
	Forced,
	PassThrough
};

// Every time pin is activated, we record it and display this data while user hovers mouse over pin
#if !UE_BUILD_SHIPPING
struct FLOW_API FPinRecord
{
	double Time;
	FString HumanReadableTime;
	EFlowPinActivationType ActivationType;

	static FString NoActivations;
	static FString PinActivations;
	static FString ForcedActivation;
	static FString PassThroughActivation;

	FPinRecord();
	FPinRecord(const double InTime, const EFlowPinActivationType InActivationType);

private:
	FORCEINLINE static FString DoubleDigit(const int32 Number);
};
#endif

// It can represent any trait added on the specific node instance, i.e. breakpoint
USTRUCT()
struct FLOW_API FFlowPinTrait
{
	GENERATED_USTRUCT_BODY()

protected:	
	UPROPERTY()
	uint8 bTraitAllowed : 1;

	uint8 bEnabled : 1;
	uint8 bHit : 1;

public:
	FFlowPinTrait()
		: bTraitAllowed(false)
		, bEnabled(false)
		, bHit(false)
	{
	};

	explicit FFlowPinTrait(const bool bInitialState)
		: bTraitAllowed(bInitialState)
		, bEnabled(bInitialState)
		, bHit(false)
	{
	};

	void AllowTrait();
	void DisallowTrait();
	bool IsAllowed() const;

	void EnableTrait();
	void DisableTrait();
	void ToggleTrait();

	bool CanEnable() const;
	bool IsEnabled() const;

	void MarkAsHit();
	void ResetHit();
	bool IsHit() const;
};
