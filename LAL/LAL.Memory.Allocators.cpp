#include "LAL.Memory.Allocators.hpp"

#include "LAL.Casting.hpp"
#include "LAL.Declarations.hpp"
#include "LAL.Memory.hpp"
#include "LAL.Platform.hpp"
#include "OSAL.Platform.hpp"

LAL_NamespaceStart
namespace Memory
{
	HeapStatus HeapStatus::Info;

#pragma region Default Allocator API
	p<void> 
	Default_Resize_WAlign(Allocator allocator, p<void> oldMemory, sw oldSize, sw newSize, sw alignment)
	{
		{
			if (! oldMemory) 
				return allocator.Procedure(allocator.Data, EAllocOp::Allocate, 
					newSize, alignment, 
					oldMemory, oldSize, 
					DefaultAllocatorFlags
				);

			if (newSize == 0) 
			{
				allocator.Procedure(allocator.Data, EAllocOp::Free, 
					newSize, alignment, 
					oldMemory, oldSize, 
					DefaultAllocatorFlags
				);
				return nullptr;
			}

			if (newSize < oldSize) 
				newSize = oldSize;

			if (oldSize == newSize)
				return oldMemory;

			p<void> newMemory = allocator.Procedure(allocator.Data, EAllocOp::Allocate, 
				newSize, alignment, 
				oldMemory, oldSize, 
				DefaultAllocatorFlags
			);

			if (! newMemory) 
				return nullptr;

			Move(newMemory, oldMemory, min(newSize, oldSize));

			allocator.Procedure(allocator.Data, EAllocOp::Free, 
				newSize, alignment, 
				oldMemory, oldSize, 
				DefaultAllocatorFlags
			);

			return newMemory;
		}
	}
#pragma endregion Default Allocator API

#pragma region Heap
	p<void>
	Heap::Procedure(p<void> allocatorData, EAllocOp op, sw size, sw alignment, p<void> oldMemory, sw oldSize, u64 flags)
	{
		p<void> result = nullptr;

		unused_param(allocatorData);
		unused_param(oldSize);

		if constexpr (HeapAnalysisEnabled)
		{
			sw infoTypeSize = sizeof(HeapAllocInfo);
			sw infoRemainder = (infoTypeSize % alignment);
			sw trackerSize = max(infoTypeSize, alignment) + infoRemainder;

			switch (op)
			{
				case EAllocOp::Free:
					if (!oldMemory)
						break;

					p<HeapAllocInfo> allocInfo = rcast< p<HeapAllocInfo>>(oldMemory) - 1;
					HeapStatus::Info.UsedMem -= allocInfo->Size;
					HeapStatus::Info.AllocCount--;

					oldMemory = allocInfo->VirtualStart;
				break;

				case EAllocOp::Allocate:
					size += trackerSize;
				break;

				default:
					break;
			}
		}

		switch (op)
		{
			case EAllocOp::Allocate:
				result = OSAL::Alloc_WAlign(size, alignment);

				if (flags & (u32)EAllocatorFlag::ClearToZero)
					Format_WithZero<u8>(rcast< p<u8>>(result), size);
			break;

			case EAllocOp::Free:
				OSAL::Free(oldMemory);
			break;

			case EAllocOp::Resize:
				result = OSAL::Resize_WAlign(oldMemory, oldSize, size, alignment);
			break;

			case EAllocOp::FreeAll:
				OSAL::Free_All();
			break;
		}

		if constexpr (HeapAnalysisEnabled)
		{
			sw infoTypeSize		= sizeof(HeapAllocInfo);
			sw infoRemainder	= (infoTypeSize % alignment);
			sw trackerSize		= max(infoTypeSize, alignment) + infoRemainder;

			if (op == EAllocOp::Allocate)
			{
				p<HeapAllocInfo> allocInfo = rcast< p<HeapAllocInfo>>(oldMemory) - 1;
				Format_ZeroItem(allocInfo);

				allocInfo->Size = size - trackerSize;
				allocInfo->VirtualStart = result;

				result = rcast< p<void>>(allocInfo + 1);

				HeapStatus::Info.UsedMem += allocInfo->Size;
				HeapStatus::Info.AllocCount++;
			}
		}

		return result;
	}
#pragma endregion Heap

#pragma region Arena
	p<void>
	Arena::Procedure(p<void> allocatorData, EAllocOp op, sw size, sw alignment, p<void> oldMemory, sw oldSize, u64 flags)
	{
		p<Arena>	arena	= rcast< p<Arena>>(allocatorData);
		p<void>		result	= nullptr;

		unused_param(oldSize);

		switch (op)
		{
			case EAllocOp::Allocate:
			{
				p<void> end			= Ptr_AddBytes(arena->VirtualStart, arena->TotalAllocated);
				sw		TotalSize	= size + alignment;

				// Out of memory
				if (arena->TotalAllocated + TotalSize > arena->TotalSize)
				{
					// TODO: Replace with just an error message when ready.
					assert(false);

					return nullptr;
				}

				result	 = Align_Forward(end, alignment);
				arena	+= arena->TotalSize;

				if (flags & u64(EAllocatorFlag::ClearToZero))
					Format_WithZero<u8>(rcast< p<u8>>(result), size);

			}
			break;

			case EAllocOp::Free:
				// Do nothing.
			break;

			case EAllocOp::FreeAll:
				arena->TotalAllocated = 0;
			break;

			case EAllocOp::Resize:
				// TODO: Check if result is on top of stack and just extend
				result = Default_Resize_WAlign(arena->Backing, oldMemory, oldSize, size, alignment);
			break;
		}

		return result;
	}
#pragma endregion Arena
}
LAL_NamespaceEnd
