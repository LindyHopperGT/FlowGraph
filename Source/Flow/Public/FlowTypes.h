// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "GameplayTagContainer.h"
#include "FlowTypes.generated.h"

#if WITH_EDITORONLY_DATA
UENUM(BlueprintType)
enum class EFlowNodeStyle : uint8
{
	Condition,
	Default,
	InOut UMETA(Hidden),
	Latent,
	Logic,
	SubGraph UMETA(Hidden),
	Custom
};
#endif

UENUM(BlueprintType)
enum class EFlowNodeState : uint8
{
	NeverActivated,
	Active,
	Completed,
	Aborted
};

// Finish Policy value is read by Flow Node
// Nodes have opportunity to terminate themselves differently if Flow Graph has been aborted
// Example: Spawn node might despawn all actors if Flow Graph is aborted, not completed
UENUM(BlueprintType)
enum class EFlowFinishPolicy : uint8
{
	Keep,
	Abort
};

UENUM(BlueprintType)
enum class EFlowSignalMode : uint8
{
	Enabled		UMETA(ToolTip = "Default state, node is fully executed."),
	Disabled	UMETA(ToolTip = "No logic executed, any Input Pin activation is ignored. Node instantly enters a deactivated state."),
	PassThrough UMETA(ToolTip = "Internal node logic not executed. All connected outputs are triggered, node finishes its work.")
};

UENUM(BlueprintType)
enum class EFlowNetMode : uint8
{
	Any					UMETA(ToolTip = "Any networking mode."),
	Authority			UMETA(ToolTip = "Executed on the server or in the single-player (standalone)."),
	ClientOnly			UMETA(ToolTip = "Executed locally, on the single client."),
	ServerOnly			UMETA(ToolTip = "Executed on the server."),
	SinglePlayerOnly	UMETA(ToolTip = "Executed only in the single player, not available in multiplayer.")
};

UENUM(BlueprintType)
enum class EFlowTagContainerMatchType : uint8
{
	HasAny				UMETA(ToolTip = "Check if container A contains ANY of the tags in the specified container B."),
	HasAnyExact			UMETA(ToolTip = "Check if container A contains ANY of the tags in the specified container B, only allowing exact matches."),
	HasAll				UMETA(ToolTip = "Check if container A contains ALL of the tags in the specified container B."),
	HasAllExact			UMETA(ToolTip = "Check if container A contains ALL of the tags in the specified container B, only allowing exact matches")
};

namespace FlowTypes
{
	FORCEINLINE_DEBUGGABLE bool HasMatchingTags(const FGameplayTagContainer& Container, const FGameplayTagContainer& OtherContainer, const EFlowTagContainerMatchType MatchType)
	{
		switch (MatchType)
		{
			case EFlowTagContainerMatchType::HasAny:
				return Container.HasAny(OtherContainer);
			case EFlowTagContainerMatchType::HasAnyExact:
				return Container.HasAnyExact(OtherContainer);
			case EFlowTagContainerMatchType::HasAll:
				return Container.HasAll(OtherContainer);
			case EFlowTagContainerMatchType::HasAllExact:
				return Container.HasAllExact(OtherContainer);
			default:
				return false;
		}
	}
}

UENUM(BlueprintType)
enum class EFlowOnScreenMessageType : uint8
{
	Temporary,
	Permanent
};

UENUM(BlueprintType)
enum class EFlowAddOnAcceptResult : uint8
{
	// Note that these enum values are ordered by priority, where greater numerical values are higher priority
	// (see CombineFlowAddOnAcceptResult)

	// No result from the current operation
	Undetermined,

	// Accept, if all other conditions are met
	TentativeAccept,

	// Reject the AddOn outright, regardless if previously TentativelyAccept-ed
	Reject,

	Max UMETA(Hidden),
	Invalid UMETA(Hidden),
	Min = Undetermined UMETA(Hidden),
};

FORCEINLINE_DEBUGGABLE EFlowAddOnAcceptResult CombineFlowAddOnAcceptResult(EFlowAddOnAcceptResult Result0, EFlowAddOnAcceptResult Result1)
{
	const __underlying_type(EFlowAddOnAcceptResult) Result0AsInt = static_cast<__underlying_type(EFlowAddOnAcceptResult)>(Result0);
	const __underlying_type(EFlowAddOnAcceptResult) Result1AsInt = static_cast<__underlying_type(EFlowAddOnAcceptResult)>(Result1);

	// Prioritize the higher numerical value enum value
	return static_cast<EFlowAddOnAcceptResult>(FMath::Max(Result0AsInt, Result1AsInt));
}
