// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Templates/SubclassOf.h"

#include "Interfaces/FlowCoreExecutableInterface.h"
#include "Interfaces/FlowContextPinSupplierInterface.h"
#include "FlowMessageLog.h"
#include "FlowTypes.h"

#include "FlowNodeBase.generated.h"

class UFlowAsset;
class UFlowNode;
class UFlowNodeAddOn;
class UFlowSubsystem;
class UEdGraphNode;
class IFlowOwnerInterface;

#if WITH_EDITOR
DECLARE_DELEGATE(FFlowNodeEvent);
#endif

typedef TFunction<void(const UFlowNodeAddOn&)> FConstFlowNodeAddOnFunction;
typedef TFunction<void(UFlowNodeAddOn&)> FFlowNodeAddOnFunction;

/**
 * The base class for UFlowNode and UFlowNodeAddOn, with their shared functionality
 */
UCLASS(Abstract, HideCategories = Object)
class FLOW_API UFlowNodeBase 
	: public UObject
	, public IFlowCoreExecutableInterface
	, public IFlowContextPinSupplierInterface
{
	GENERATED_UCLASS_BODY()

	// UObject
	virtual UWorld* GetWorld() const override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// --

	// IFlowCoreExecutableInterface
	virtual void InitializeInstance();
	virtual void DeinitializeInstance();
	virtual void PreloadContent();
	virtual void FlushContent();
	virtual void OnActivate();
	virtual void Cleanup();
	virtual void ForceFinishNode();
	virtual void ExecuteInput(const FName& PinName);
	// --

	// UFlowNodeBase
	virtual bool IsSupportedInputPinName(const FName& PinName) const PURE_VIRTUAL(IsSupportedInputPinName, return true;);

protected:
	// FlowNodes and AddOns may determine which AddOns are eligible to be their children
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	EFlowAddOnAcceptResult AcceptFlowNodeAddOnChild(const UFlowNodeAddOn* AddOnTemplate) const;

public:
#if WITH_EDITOR
	EFlowAddOnAcceptResult CheckAcceptFlowNodeAddOnChild(const UFlowNodeAddOn* AddOnTemplate) const;
	virtual TArray<UFlowNodeAddOn*>& GetFlowNodeAddOnChildrenByEditor() { return AddOns; }
#endif // WITH_EDITOR
	virtual const TArray<UFlowNodeAddOn*>& GetFlowNodeAddOnChildren() const { return AddOns; }

	virtual UFlowNode* GetFlowNodeSelfOrOwner() PURE_VIRTUAL(GetFlowNodeSelfOrOwner, return nullptr;);
	const UFlowNode* GetFlowNodeSelfOrOwner() const { return const_cast<UFlowNodeBase*>(this)->GetFlowNodeSelfOrOwner(); }

	// Call a Function for all of this object's AddOns (including all AddOn's AddOns, ie recursively)
	void ForEachAddOnConst(FConstFlowNodeAddOnFunction Function) const;
	void ForEachAddOn(FFlowNodeAddOnFunction Function) const;
	// --

	UFUNCTION(BlueprintPure, Category = "FlowNode")
	UFlowAsset* GetFlowAsset() const;

	UFUNCTION(BlueprintPure, Category = "FlowNode")
	UFlowSubsystem* GetFlowSubsystem() const;

	// Gets the Owning Actor for this Node's RootFlow
	// (if the immediate parent is an UActorComponent, it will get that Component's actor)
	UFUNCTION(BlueprintCallable, Category = "FlowNode")
	AActor* TryGetRootFlowActorOwner() const;

	// Gets the Owning Object for this Node's RootFlow
	UFUNCTION(BlueprintCallable, Category = "FlowNode")
	UObject* TryGetRootFlowObjectOwner() const;

	// Returns the IFlowOwnerInterface for the owner object (if implemented)
	//  NOTE - will consider a UActorComponent owner's owning actor if appropriate
	IFlowOwnerInterface* GetFlowOwnerInterface() const;

protected:

	// Helper functions for GetFlowOwnerInterface()
	IFlowOwnerInterface* TryGetFlowOwnerInterfaceFromRootFlowOwner(UObject& RootFlowOwner, const UClass& ExpectedOwnerClass) const;
	IFlowOwnerInterface* TryGetFlowOwnerInterfaceActor(UObject& RootFlowOwner, const UClass& ExpectedOwnerClass) const;

public:

	// Set the editor-only Config Text 
	// (for displaying config info on the Node in the flow graph, ignored in non-editor builds)
	UFUNCTION(BlueprintCallable)
	void SetNodeConfigText(const FText& NodeConfigText);

	// Called whenever a property change event occurs on this flow node object,
	// giving the implementor a chance to update their NodeConfigText (via SetNodeConfigText)
	UFUNCTION(BlueprintNativeEvent)
	void UpdateNodeConfigText();

#if WITH_EDITOR
	UEdGraphNode* GetGraphNode() const { return GraphNode; }

	// Opportunity to update node's data before UFlowGraphNode would call ReconstructNode()
	virtual void FixNode(UEdGraphNode* NewGraphNode);

	void SetGraphNode(UEdGraphNode* NewGraphNode);
	
	// Setup the UFlowNodeBase when being opened for edit in the editor
	virtual void SetupForEditing(UEdGraphNode& EdGraphNode);

	virtual FString GetNodeCategory() const;
	virtual FText GetNodeTitle() const;
	virtual FText GetNodeToolTip() const;
	virtual FText GetNodeConfigText() const;

	// This method allows to have different for every node instance, i.e. Red if node represents enemy, Green if node represents a friend
	virtual bool GetDynamicTitleColor(FLinearColor& OutColor) const;

	EFlowNodeStyle GetNodeStyle() const { return NodeStyle; }

	// Short summary of node's content - displayed over node as NodeInfoPopup
	virtual FString GetNodeDescription() const;

	// used when import graph from another asset
	virtual void PostImport() {}

	// IFlowContextPinSupplierInterface
	virtual bool SupportsContextPins() const override { return false; }
	virtual TArray<FFlowPin> GetContextInputs() const override;
	virtual TArray<FFlowPin> GetContextOutputs() const override;
	// --

#endif // WITH_EDITOR

	static const FFlowPin* FindFlowPinByName(const FName& PinName, const TArray<FFlowPin>& FlowPins);

	UFUNCTION(BlueprintCallable, Category = "FlowNode", meta = (DevelopmentOnly))
	void LogError(FString Message, const EFlowOnScreenMessageType OnScreenMessageType = EFlowOnScreenMessageType::Permanent);

	// LogError from constant function (allowing this to be modified only to log the error itself)
	FORCEINLINE void LogErrorConst(FString Message, const EFlowOnScreenMessageType OnScreenMessageType = EFlowOnScreenMessageType::Permanent) const
		{ const_cast<UFlowNodeBase*>(this)->LogError(Message, OnScreenMessageType); }

	UFUNCTION(BlueprintCallable, Category = "FlowNode", meta = (DevelopmentOnly))
	void LogWarning(FString Message);

	// LogWarning from constant function (allowing this to be modified only to log the warning itself)
	FORCEINLINE void LogWarningConst(FString Message) const { const_cast<UFlowNodeBase*>(this)->LogWarning(Message); }

	UFUNCTION(BlueprintCallable, Category = "FlowNode", meta = (DevelopmentOnly))
	void LogNote(FString Message);

	// LogNote from constant function (allowing this to be modified only to log the note itself)
	FORCEINLINE void LogNoteConst(FString Message) const { const_cast<UFlowNodeBase*>(this)->LogNote(Message); }

#if !UE_BUILD_SHIPPING
private:
	bool BuildMessage(FString& Message) const;
#endif

protected:
	// Short summary of node's content - displayed over node as NodeInfoPopup
	UFUNCTION(BlueprintImplementableEvent, Category = "FlowNode", meta = (DisplayName = "Get Node Description"))
	FString K2_GetNodeDescription() const;

protected:
	// Flow Node AddOn attachments
	UPROPERTY(BlueprintReadOnly, Instanced, Category = "Configuration")
	TArray<UFlowNodeAddOn*> AddOns;

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	UEdGraphNode* GraphNode = nullptr;

public:
	UPROPERTY(EditDefaultsOnly, Category = "FlowNode")
	EFlowNodeStyle NodeStyle = EFlowNodeStyle::Default;

	// Set Node Style to custom to use your own color for this node
	UPROPERTY(EditDefaultsOnly, Category = "FlowNode", meta = (EditCondition = "NodeStyle == EFlowNodeStyle::Custom"))
	FLinearColor NodeColor = FLinearColor::Black;

	uint8 bCanDelete : 1 = true;
	uint8 bCanDuplicate : 1 = true;

	UPROPERTY()
	FString Category;

	// Optional developer-facing text to explain the configuration of this node when viewed in the editor
	// may be authored or set procedurally via UpdateNodeConfigText and SetNodeConfigText
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category = "Configuration")
	FText DevNodeConfigText = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, Category = "FlowNode")
	bool bNodeDeprecated = false;

	// If this node is deprecated, it might be replaced by another node
	UPROPERTY(EditDefaultsOnly, Category = "FlowNode")
	TSubclassOf<UFlowNode> ReplacedBy;

	FFlowNodeEvent OnReconstructionRequested;

	FFlowMessageLog ValidationLog;

#endif // WITH_EDITORONLY_DATA
};
