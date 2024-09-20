// Copyright Riot Games, All Rights Reserved.

#pragma once

#include "Containers/Array.h"

#include "Math/RandomStream.h"

namespace FlowArray
{
	// Alias for inline-allocated TArray 
	//  (NOTE, UE's TArray will reallocate to heap ("secondary allocation")
	//   if the fixed capacity is ever exceeded)

	template <class T, int Capacity>
	using TInlineArray = TArray<T, TInlineAllocator<Capacity>>;

	template <class T, typename InAllocatorType>
	void ReverseArray(TArray<T>& InOutArray)
	{
		for (int32 FrontIndex = 0, BackIndex = InOutArray.Num() - 1; FrontIndex < BackIndex; ++FrontIndex, --BackIndex)
		{
			InOutArray.Swap(FrontIndex, BackIndex);
		}
	}


	template <typename T, typename InAllocatorType>
	void ShuffleArray(TArray<T, InAllocatorType>& Array, FRandomStream& RandomStream)
	{
		// Trivial cases
		if (Array.Num() <= 2)
		{
			if (Array.Num() == 2)
			{
				const bool bShouldSwap = RandomStream.RandRange(0, 1) == 0;
				if (bShouldSwap)
				{
					Array.Swap(0, 1);
				}
			}

			return;
		}

		// Simple shuffle, attempt swaps for each index in the array, once each
		for (int32 FromIndex = 0; FromIndex < Array.Num(); ++FromIndex)
		{
			const int32 IndexOffset = RandomStream.RandRange(1, Array.Num() - 1);
			const int32 OtherIndex = (FromIndex + IndexOffset) % Array.Num();
			check(FromIndex != OtherIndex);

			Array.Swap(FromIndex, OtherIndex);
		}
	}
}