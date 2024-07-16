// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Graph/FlowGraphSettings.h"

#include "FlowAsset.h"
#include "Algo/Unique.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Graph/FlowGraphSchema.h"
#include "Widgets/Notifications/SNotificationList.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowGraphSettings)

#define LOCTEXT_NAMESPACE "FlowGraphSettings"

UFlowGraphSettings::UFlowGraphSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bExposeFlowAssetCreation(true)
	, bExposeFlowNodeCreation(true)
	, bShowAssetToolbarAboveLevelEditor(true)
	, FlowAssetCategoryName(LOCTEXT("FlowAssetCategory", "Flow"))
	, DefaultFlowAssetClass(UFlowAsset::StaticClass())
	, WorldAssetClass(UFlowAsset::StaticClass())
	, bShowDefaultPinNames(false)
	, ExecPinColorModifier(0.75f, 0.75f, 0.75f, 1.0f)
	, NodeDescriptionBackground(FLinearColor(0.0625f, 0.0625f, 0.0625f, 1.0f))
	, NodeStatusBackground(FLinearColor(0.12f, 0.12f, 0.12f, 1.0f))
	, NodePreloadedBackground(FLinearColor(0.12f, 0.12f, 0.12f, 1.0f))
	, ConnectionDrawType(EFlowConnectionDrawType::Default)
	, CircuitConnectionAngle(45.f)
	, CircuitConnectionSpacing(FVector2D(30.f))
	, InactiveWireColor(FLinearColor(0.364f, 0.364f, 0.364f, 1.0f))
	, InactiveWireThickness(1.5f)
	, RecentWireDuration(3.0f)
	, RecentWireColor(FLinearColor(1.0f, 0.05f, 0.0f, 1.0f))
	, RecentWireThickness(6.0f)
	, RecordedWireColor(FLinearColor(0.432f, 0.258f, 0.096f, 1.0f))
	, RecordedWireThickness(3.5f)
	, SelectedWireColor(FLinearColor(0.984f, 0.482f, 0.010f, 1.0f))
	, SelectedWireThickness(1.5f)
{
	NodeTitleColors.Emplace(EFlowNodeStyle::Condition, FLinearColor(1.0f, 0.62f, 0.016f, 1.0f));
	NodeTitleColors.Emplace(EFlowNodeStyle::Default, FLinearColor(-0.728f, 0.581f, 1.0f, 1.0f));
	NodeTitleColors.Emplace(EFlowNodeStyle::InOut, FLinearColor(1.0f, 0.0f, 0.008f, 1.0f));
	NodeTitleColors.Emplace(EFlowNodeStyle::Latent, FLinearColor(0.0f, 0.770f, 0.375f, 1.0f));
	NodeTitleColors.Emplace(EFlowNodeStyle::Logic, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	NodeTitleColors.Emplace(EFlowNodeStyle::SubGraph, FLinearColor(1.0f, 0.128f, 0.0f, 1.0f));

	NodePrefixesToRemove.Emplace("FN");
	NodePrefixesToRemove.Emplace("FlowNode");
	NodePrefixesToRemove.Emplace("FlowNodeAddOn");
}

void UFlowGraphSettings::PostInitProperties()
{
	Super::PostInitProperties();

	NodePrefixesToRemove.Sort(TGreater{});
}

#if WITH_EDITOR

void UFlowGraphSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED( UFlowGraphSettings, NodePrefixesToRemove ))
	{
		//
		// We need to sort items in array, because unsorted array can cause only partial prefix removal.
		// For example, we have a NodeName = "UFlowNode_Custom" and these elements in the array:
		//		NodePrefixesToRemove = {"FN", "Flow", "FlowNode"};
		// Note: Prefix "U" is removed by Unreal
		// 
		//	First prefix does not match start of NodeName, so nothing changes.
		//	Second prefix match start of the name and is removed, NodeName becomes "Node_Custom"
		//  Third prefix does not match start of NodeName, so nothing changes.
		//  After complete process NodeName == "Node_Custom", but expected result is "Custom"
		//
		// If NodePrefixesToRemove = {"FN", "FlowNode", "Flow"} instead, everything will be removed as expected.
		//
		
		NodePrefixesToRemove.Sort(TGreater{});
		
		const int32 SizeBefore = NodePrefixesToRemove.Num();
		const int32 SizeAfter = Algo::Unique(NodePrefixesToRemove);
	
		if (SizeBefore > SizeAfter)
		{
			NodePrefixesToRemove.SetNum(SizeAfter);
			
			// error notification
			FNotificationInfo Info(LOCTEXT("FlowGraphSettings_DuplicatePrefixError", "Added prefix already exists in array."));
			Info.ExpireDuration = 3.0f;
			FSlateNotificationManager::Get().AddNotification(Info)->SetCompletionState(SNotificationItem::CS_Fail);
		}
		else
		{
			UFlowGraphSchema::UpdateGeneratedDisplayNames();
		}
	}
}
#endif

#undef LOCTEXT_NAMESPACE
