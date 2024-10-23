// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

class FString;
class UClass;

#if WITH_EDITOR
namespace FlowClassUtils
{
	TArray<UClass*> GetClassesFromMetadataString(const FString& MetadataString);
}
#endif