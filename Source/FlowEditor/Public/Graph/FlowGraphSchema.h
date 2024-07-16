// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "Templates/SubclassOf.h"
#include "FlowGraphSchema.generated.h"

class UFlowAsset;
class UFlowNode;
class UFlowNodeAddOn;
class UFlowNodeBase;
class UFlowGraphNode;

DECLARE_MULTICAST_DELEGATE(FFlowGraphSchemaRefresh);

UCLASS()
class FLOWEDITOR_API UFlowGraphSchema : public UEdGraphSchema
{
	GENERATED_UCLASS_BODY()

	friend class UFlowGraph;

private:
	static bool bInitialGatherPerformed;
	static TArray<UClass*> NativeFlowNodes;
	static TArray<UClass*> NativeFlowNodeAddOns;
	static TMap<FName, FAssetData> BlueprintFlowNodes;
	static TMap<FName, FAssetData> BlueprintFlowNodeAddOns;
	static TMap<TSubclassOf<UFlowNodeBase>, TSubclassOf<UEdGraphNode>> GraphNodesByFlowNodes;

	static bool bBlueprintCompilationPending;

public:
	static void SubscribeToAssetChanges();
	static void GetPaletteActions(FGraphActionMenuBuilder& ActionMenuBuilder, const UFlowAsset* EditedFlowAsset, const FString& CategoryName);

	// EdGraphSchema
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual const FPinConnectionResponse CanMergeNodes(const UEdGraphNode* NodeA, const UEdGraphNode* NodeB) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual bool ShouldHidePinDefaultValue(UEdGraphPin* Pin) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual FText GetPinDisplayName(const UEdGraphPin* Pin) const override;
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
	virtual int32 GetNodeSelectionCount(const UEdGraph* Graph) const override;
	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
	virtual void OnPinConnectionDoubleCicked(UEdGraphPin* PinA, UEdGraphPin* PinB, const FVector2D& GraphPosition) const override;
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	virtual int32 GetCurrentVisualizationCacheID() const override;
	virtual void ForceVisualizationCacheClear() const override;
	// --

	// FlowGraphSchema
	virtual void GetGraphNodeContextActions(FGraphContextMenuBuilder& ContextMenuBuilder, int32 SubNodeFlags) const;

	bool IsAddOnAllowedForSelectedObjects(const TArray<UObject*>& SelectedObjects, const UFlowNodeAddOn* AddOnTemplate) const;

		// --

	static void UpdateGeneratedDisplayNames();
	static void UpdateGeneratedDisplayName(UClass* NodeClass, bool bBatch = false);

	static TArray<TSharedPtr<FString>> GetFlowNodeCategories();
	static TSubclassOf<UEdGraphNode> GetAssignedGraphNodeClass(const TSubclassOf<UFlowNodeBase>& FlowNodeClass);

	static bool IsPIESimulating();

protected:
	static UFlowGraphNode* CreateDefaultNode(UEdGraph& Graph, const UFlowAsset* AssetClassDefaults, const TSubclassOf<UFlowNode>& NodeClass, const FVector2D& Offset, bool bPlacedAsGhostNode);

private:
	static void ApplyNodeOrAddOnFilter(const UFlowAsset* AssetClassDefaults, const UClass* FlowNodeClass, TArray<UFlowNodeBase*>& FilteredNodes);
	static void GetFlowNodeActions(FGraphActionMenuBuilder& ActionMenuBuilder, const UFlowAsset* EditedFlowAsset, const FString& CategoryName);
	static TArray<UFlowNodeBase*> GetFilteredPlaceableNodesOrAddOns(const UFlowAsset* EditedFlowAsset, const TArray<UClass*>& InNativeNodesOrAddOns, const TMap<FName, FAssetData>& InBlueprintNodesOrAddOns);

	static void GetCommentAction(FGraphActionMenuBuilder& ActionMenuBuilder, const UEdGraph* CurrentGraph = nullptr);

	static bool IsFlowNodeOrAddOnPlaceable(const UClass* Class);

	static void OnBlueprintPreCompile(UBlueprint* Blueprint);
	static void OnBlueprintCompiled();
	static void OnHotReload(EReloadCompleteReason ReloadCompleteReason);

	static void GatherNativeNodesOrAddOns(const TSubclassOf<UFlowNodeBase>& FlowNodeBaseClass, TArray<UClass*>& InOutNodesOrAddOnsArray);
	static void GatherNodes();

	static void OnAssetAdded(const FAssetData& AssetData);
	static void AddAsset(const FAssetData& AssetData, const bool bBatch);
	static bool ShouldAddToBlueprintFlowNodesMap(const FAssetData& AssetData, const TSubclassOf<UBlueprint>& BlueprintClass, const TSubclassOf<UFlowNodeBase>& FlowNodeBaseClass);

	static void OnAssetRemoved(const FAssetData& AssetData);
	static void OnAssetRenamed(const FAssetData& AssetData, const FString& OldObjectPath);

public:
	static FFlowGraphSchemaRefresh OnNodeListChanged;
	static UBlueprint* GetPlaceableNodeOrAddOnBlueprint(const FAssetData& AssetData);

	static const UFlowAsset* GetEditedAssetOrClassDefault(const UEdGraph* Graph);

private:
	// ID for checking dirty status of node titles against
	static int32 CurrentCacheRefreshID;
};
