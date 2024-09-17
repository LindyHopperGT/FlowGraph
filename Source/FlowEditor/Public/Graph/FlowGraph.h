// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "EdGraph/EdGraph.h"

#include "FlowAsset.h"
#include "FlowGraph.generated.h"

class SFlowGraphEditor;
class UFlowGraphNode;
class UFlowGraphSchema;

class FLOWEDITOR_API FFlowGraphInterface : public IFlowGraphInterface
{
public:
	virtual ~FFlowGraphInterface() override {}

	virtual void OnInputTriggered(UEdGraphNode* GraphNode, const int32 Index) const override;
	virtual void OnOutputTriggered(UEdGraphNode* GraphNode, const int32 Index) const override;
};

UCLASS()
class FLOWEDITOR_API UFlowGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

public:
	static UEdGraph* CreateGraph(UFlowAsset* InFlowAsset);
	static UEdGraph* CreateGraph(UFlowAsset* InFlowAsset, TSubclassOf<UFlowGraphSchema> FlowSchema);
	void RefreshGraph();

	// UEdGraph
	virtual void NotifyGraphChanged() override;
	// --

	/** Returns the FlowAsset that contains this graph */
	UFlowAsset* GetFlowAsset() const;

	//~ Begin UObject Interface.
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface.

	virtual void CollectAllNodeInstances(TSet<UObject*>& NodeInstances);
	virtual bool CanRemoveNestedObject(UObject* TestObject) const;
	virtual void OnNodeInstanceRemoved(UObject* NodeInstance);

	UEdGraphPin* FindGraphNodePin(UEdGraphNode* Node, EEdGraphPinDirection Dir);
	
public:

	virtual void OnCreated();
	virtual void OnLoaded();
	virtual void OnSave();

	virtual void Initialize();

	virtual void UpdateAsset(int32 UpdateFlags = 0);
	virtual void UpdateVersion();
	virtual void MarkVersion();

	virtual void OnSubNodeDropped();
	virtual void OnNodesPasted(const FString& ImportStr);

	bool UpdateUnknownNodeClasses();
	void UpdateDeprecatedClasses();
	void RemoveOrphanedNodes();
	void UpdateClassData();

	bool IsLocked() const;
	void LockUpdates();
	void UnlockUpdates();

protected:
	static void RecursivelySetupAllFlowGraphNodesForEditing(UFlowGraphNode& FromFlowGraphNode);
	void RecursivelyRefreshAddOns(UFlowGraphNode& FromFlowGraphNode);

	static FString GetDeprecationMessage(const UClass* Class);
	static void UpdateFlowGraphNodeErrorMessage(UFlowGraphNode& Node);

protected:

	/** Graph version number */
	UPROPERTY()
	int32 GraphVersion;
	
	/** if set, graph modifications won't cause updates in internal tree structure
	 *  flag allows freezing update during heavy changes like pasting new nodes 
	 */
	uint32 bLockUpdates : 1;
};
