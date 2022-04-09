#pragma once

#define LAL_Config_EnableHeapAnalysis

#include "LAL.Bits.hpp"
#include "LAL.Metaprogramming.hpp"
#include "LAL.Types.hpp"
#include "OSAL.Platform.hpp"

#ifndef HPP_LAL_Memory_Allocators
LAL_NamespaceStart
namespace Memory {

enum class 
EAllocOp : u32
{
	Allocate,
	Free,
	FreeAll,
	Resize
};

enum class 
EAllocatorFlag : sw
{
	ClearToZero = bit(0)
};


using AllocatorProc = p<void>(p<void> data, EAllocOp op, sw size, sw alignment, p<void> oldMemory, sw oldSize, u64 flags);

constexpr sw	DefaultAlignment		= OSAL::DefaultAlignment;
constexpr u32 	DefaultAllocatorFlags	= u32(EAllocatorFlag::ClearToZero);


struct Allocator
{
	p<AllocatorProc>	Procedure;
	p<void>				Data;

#pragma region DefaultAPI
	// Allocate memory with default alignment.
	template<class Type> p<Type>
	Alloc(sw num) {
		return Alloc_WithAlign<Type>(num, DefaultAlignment);
	}

	// Allocate memory with specified alignment.
	template<class Type> p<Type>
	Alloc_WithAlign(sw num, sw alignment)
	{
		if constexpr (! IsSame<Type, void>())
			num *= sizeof(Type);

		if (Procedure)
			Procedure(Data, EAllocOp::Allocate, num, alignment, nullptr, 0, DefaultAllocatorFlags);
	}

	// Allocate memory and copy data into it.
	template<class Type> p<Type>
	Alloc_Copy(p<void> source, sw size)	{
		return Alloc_Copy_WithAlign<Type>(source, size, DefaultAlignment);
	}

	// Allocate memory and copy data into it with specified alignment.
	template<class Type> p<Type>
	Alloc_Copy_WithAlign(p<void> source, sw num, sw alignment) {
		return Copy<Type>(Alloc_WithAlign(num, alignment), source, num);
	}

	// Free allocated memory.
	template<class Type> void
	Free(p<Type> ptr) {
		if (Procedure)
			Procedure(Data, EAllocOp::Free, 0, 0, ptr, 0, DefaultAllocatorFlags);
	}

	// Free all memory allocated by an allocator.
	template<class Type> void
	Free_All() {
		if (Procedure)
			Procedure(Data, EAllocOp::FreeAll, 0, 0, nullptr, 0, DefaultAllocatorFlags);
	}

	// Resize allocated memory.
	template<class Type> p<Type>
	Resize(p<void> ptr, sw oldSize, sw newSize)	{
		return Resize_WAlign(ptr, oldSize, newSize, DefaultAlignment);
	}

	// Resize allocated memory with specified alignment.
	template<class Type> p<Type>	
	Resize_WAlign(p<void> ptr, sw oldSize, sw newSize, sw alignment) {
		if (Procedure)
			return Procedure(Data, EAllocOp::Resize, newSize, alignment, ptr, oldSize, DefaultAllocatorFlags);
	}
#pragma region DefaultAPI
};

#pragma region Default Allocator API
p<void> Default_Resize_WAlign(Allocator allocator, p<void> oldMemory, sw oldSize, sw newSize, sw alignment);

// Uses the OS system's memory manager. (Heap Allocator)

// Allocate memory with default alignment.
template<class Type> p<Type>
Alloc(sw num) {
	return Alloc_WithAlign<Type>(num, DefaultAlignment);
}

// Allocate memory with specified alignment.
template<class Type> p<Type>
Alloc_WithAlign(sw num, sw alignment) {
	return rcast< p<Type>>(OSAL::Alloc_WithAlign(num * sizeof(Type), alignment));
}

// Allocate memory of Type and copy providedAddr to it. (Assumes objects are stored in contigous space)
// Warning: This does not perform initialization!
template<class Type> p<Type> 
Alloc_Copy(p<Type> source, sw num) {
	return Copy<Type>(Alloc<Type>(num), source, num);
}

// Allocate memory of Type and copy providedAddr to it (with specified alignment). (Assumes objects are stored in contigous space)
// Warning: This does not perform initialization!
template<class Type> p<Type> 
Alloc_Copy_WithAlign(p<void> source, sw num, sw alignment)	{
	return Copy<Type>(OSAL::Alloc_WithAlign(num, alignment), source, num);
}

// Free allocated memory.
template<class Type> void
Free(p<void> ptr) {
	OSAL::Free(rcast< p<void>>(ptr));
}

// Resize allocated memory.
template<class Type> p<Type>
Resize(p<void> ptr, sw oldSize, sw newSize)	{
	return OSAL::Resize_WAlign(ptr, oldSize * sizeof(Type), newSize * sizeof(Type), DefaultAlignment);
}

// Resize allocated memory with specified alignment.
template<class Type> p<Type>
Resize_WAlign(p<void> ptr, sw oldSize, sw newSize, sw alignment) {
	return OSAL::Resize_WAlign(ptr, oldSize * sizeof(Type), newSize * sizeof(Type), alignment);
}
#pragma endregion Default Allocator API

#ifdef LAL_Config_EnableHeapAnalysis
constexpr bool HeapAnalysisEnabled = false;
#endif

#pragma region Heap
struct Heap : Allocator
{
	static Allocator GetAllocator()	{
		return { Procedure, nullptr };
	};

protected:
	static p<void> 
	Procedure(p<void> Data, EAllocOp op, sw size, sw alignment, p<void> oldMemory, sw oldSize, u64 flags);
};

struct HeapStatus
{
	friend Heap;
	constexpr static u32 Magic_DeadCode = 0xDEADC0DE;

#pragma region Data
	uw Magic;
	sw AllocCount;
	sw UsedMem;
#pragma endregion Data

	static void Init();

	static sw	AllocationCount();
	static void Check();
	static sw	UsedMemory();

protected:
	static HeapStatus 
	Info;
};

struct HeapAllocInfo
{
	sw		Size;
	p<void> VirtualStart;
};
#pragma endregion Heap

#pragma region Arena
struct Arena
{
#pragma region Vars
	Allocator	Backing;
	sw			TotalSize;
	sw			TotalAllocated;
	sw			NumOpenSnapshots;
	p<void>		VirtualStart;
#pragma endregion Vars

	void Init_FromMemory		(p<void> providedMem, sw size);
	void Init_FromAllocator	(Allocator BackingAlloc, sw size);
	void Init_AsSub			(Arena& Parent, sw size);

	sw AlignmentOf(sw alignment);
	sw SizeRemaining(sw alignment);

	void ReleaseBacking();

#pragma region DefaultAPI
	static Allocator 
	GetAllocator(Arena& arena) {
		return { Procedure, rcast<p<void>>(ptrof arena) };
	}

protected:
	static p<void>
	Procedure(p<void> Data, EAllocOp op, sw size, sw alignment, p<void> oldMemory, sw oldSize, u64 flags);
public:

	// Allocate memory with default alignment.
	template<class Type> p<Type>
	Alloc(sw num) {
		return Alloc_WithAlign(num, DefaultAlignment);
	}

	// Allocate memory with specified alignment.
	template<class Type> p<Type>
	Alloc_WithAlign(sw num, sw alignment) {
		if constexpr (! IsSame<Type, void>())
			num *= sizeof(Type);

		return Procedure(Backing.Data, EAllocOp::Allocate, num, alignment, nullptr, 0, DefaultAllocatorFlags);
	}

	// Allocate memory and copy data into it.
	template<class Type> p<Type>
	Alloc_Copy(p<void> source, sw size) {
		return Alloc_Copy_WithAlign(source, size, DefaultAlignment);
	}

	// Allocate memory and copy data into it with specified alignment.
	template<class Type> p<Type>
	Alloc_Copy_WithAlign(p<void> source, sw num, sw alignment) {
		return Copy<Type>(Alloc_WithAlign(num, alignment), source, num);
	}

	// Free allocated memory.
	template<class Type> void
	Free(p<Type> ptr) {
		// Do nothing
	}

	// Free all memory allocated by the arena.
	void Free_All() {
		TotalAllocated = 0;
	}

	// Resize allocated memory.
	template<class Type> ForceInline p<Type>
	Resize(p<void> ptr, sw oldSize, sw newSize)	{
		return Resize_WAlign(ptr, oldSize, newSize, DefaultAlignment);
	}

	// Resize allocated memory with specified alignment.
	template<class Type> ForceInline p<Type>	
	Resize_WAlign(p<void> ptr, sw oldSize, sw newSize, sw alignment) {
		// TODO: Check if result is on top of stack and just extend
		return Default_Resize_WAlign(Backing, ptr, oldSize, newSize, alignment);
	}
#pragma endregion DefaultAPI
};

struct ArenaSnapshot
{
	Arena*	Ref;
	sw		OriCount;


	static ArenaSnapshot Begin(Arena& arena);

	void End();
};
#pragma endregion Arena

#pragma region Pool
struct Pool
{
#pragma region Vars
	Allocator	Backing;
	p<void>		VirtualStart;
	p<void>		OpenBlocks;
	sw			BlockSize;
	sw			BlockAlignment;
	sw			TotalSize;
	sw			NumBlocks;
#pragma region Vars

	void Init(Allocator backing, sw numBlocks, sw blockSize) {
		Init_WithAlign(backing, numBlocks, blockSize, DefaultAlignment);
	}

	void Init_WithAlign(Allocator backing, sw numBlocks, sw blockSize, sw blockAlignment);

	void ReleaseBacking() {
		if (Backing.Procedure)
			Backing.Free(VirtualStart);
	}

#pragma region DefaultAPI
	static Allocator 
	GetAllocator(Pool& pool) {
		return { Procedure, rcast<p<void>>(ptrof pool) };
	}

protected:
	static p<void>
	Procedure(p<void> Data, EAllocOp op, sw size, sw alignment, p<void> oldMemory, sw oldSize, u64 flags);
public:

	// Allocate memory with default alignment.
	template<class Type> p<Type>
	Alloc(sw num) {
		return Alloc_WithAlign(num, DefaultAlignment);
	}

	// Allocate memory with specified alignment.
	template<class Type> p<Type>
	Alloc_WithAlign(sw num, sw alignment) {
		if constexpr (! IsSame<Type, void>())
			num *= sizeof(Type);

		return Procedure(Backing.Data, EAllocOp::Allocate, num, alignment, nullptr, 0, DefaultAllocatorFlags);
	}

	// Allocate memory and copy data into it.
	template<class Type> p<Type>
	Alloc_Copy(p<void> source, sw size) {
		return Alloc_Copy_WithAlign(source, size, DefaultAlignment);
	}

	// Allocate memory and copy data into it with specified alignment.
	template<class Type> p<Type>
	Alloc_Copy_WithAlign(p<void> source, sw num, sw alignment) {
		return Copy<Type>(Alloc_WithAlign(num, alignment), source, num);
	}

	// Free allocated memory.
	template<class Type> void
	Free(p<Type> ptr) {
		Procedure(this, EAllocOp::Free, 0, 0, ptr, 0, DefaultAllocatorFlags);
	}

	// Free all memory allocated by the arena.
	void Free_All() {
		Procedure(this, EAllocOp::FreeAll, 0, 0, nullptr, 0, DefaultAllocatorFlags);
	}

	// Resize allocated memory.
	template<class Type> ForceInline p<Type>
	Resize(p<void> ptr, sw oldSize, sw newSize)	{
		return Resize_WAlign(ptr, oldSize, newSize, DefaultAlignment);
	}

	// Resize allocated memory with specified alignment.
	template<class Type> ForceInline p<Type>	
	Resize_WAlign(p<void> ptr, sw oldSize, sw newSize, sw alignment) {
		// Cannot resize
		// TODO: Panic message.
	}
#pragma endregion DefaultAPI
};
#pragma endregion Pool

#pragma region Scratch - Ring Buffer based arena
struct Scratch
{
#pragma region Vars
	p<void> VirtualStart;
	sw		TotalSize;
	p<void> AllocPoint;
	p<void> FreePoint;
#pragma endregion Vars

	void Init(p<void> start, sw size);
	
	b32 IsInUse(p<void> ptr);

#pragma region DefaultAPI
	static Allocator 
		GetAllocator(Scratch& pool) {
		return { Procedure, rcast<p<void>>(ptrof pool) };
	}

protected:
	static p<void>
	Procedure(p<void> Data, EAllocOp op, sw size, sw alignment, p<void> oldMemory, sw oldSize, u64 flags);
public:
#pragma endregion DefaultAPI
};

struct ScratchHeaderEV
{
	sw Size;


	static p<ScratchHeaderEV> Get(p<void> data);

	void Fill(p<void> data, sw size);
};
#pragma endregion Scratch - Ring Buffer based arena

#pragma region Implementation

#pragma region Heap
void 
HeapStatus::Init()
{
	Format_ZeroItem(Info);

	Info.Magic = Magic_DeadCode;
}

sw
HeapStatus::AllocationCount()
{
	//assert(Info.Magic == Magic_DeadCode, "HeapStatus is not initialized yet, call HeapStatus::Init first!");

	return Info.AllocCount;
}

void
HeapStatus::Check()
{
	//assert(Info.Magic == Magic_DeadCode, "HeapStatus is not initialized yet, call HeapStatus::Init first!");
	
	//assert(Info.AllocCount	== 0);
	//assert(Info.UsedMem		== 0);
}
#pragma endregion Heap

#pragma region Arena
ForceInline void
Arena::Init_FromMemory(p<void> providedMem, sw size)
{
	Backing				= { nullptr, nullptr };
	VirtualStart		= providedMem;
	TotalSize			= size;
	TotalAllocated		= 0;
	NumOpenSnapshots	= 0;
}

ForceInline void
Arena::Init_FromAllocator(Allocator allocator, sw size)
{
	Backing				= allocator;
	VirtualStart		= allocator.Alloc<void>(size);
	TotalSize			= size;
	TotalAllocated		= 0;
	NumOpenSnapshots	= 0;
}

ForceInline void
Arena::Init_AsSub(Arena& arena, sw size)
{
	Init_FromAllocator(GetAllocator(arena), size);
}

ForceInline void
Arena::ReleaseBacking()
{
	if (Backing.Procedure)
	{
		Backing.Free(VirtualStart);

		VirtualStart		= nullptr;
		TotalSize			= 0;
		TotalAllocated		= 0;
		NumOpenSnapshots	= 0;
	}
}

ForceInline sw
Arena::AlignmentOf(sw alignment)
{
	sw
	offset		= 0,
	resultPtr	= 0,
	mask		= 0;

	//assert(IsPowerOfTwo(alignment));

	resultPtr	= rcast<sw>(VirtualStart) + TotalAllocated;
	mask		= alignment - 1;

	if (resultPtr & mask)
		offset = alignment - (resultPtr & mask);

	return offset;
}

ForceInline sw
Arena::SizeRemaining(sw alignment)
{
	sw result = TotalSize - (TotalAllocated + AlignmentOf(alignment));

	return result;
}

ForceInline ArenaSnapshot
ArenaSnapshot::Begin(Arena& arena)
{
	ArenaSnapshot 
	snapshot { ptrof arena, arena.TotalAllocated };

	arena.NumOpenSnapshots++;
	return snapshot;
}

ForceInline void
ArenaSnapshot::End()
{
	//assert(Ref->TotalAllocated >= OriCount);
	//assert(Ref->NumOpenSnapshots > 0);

	Ref->TotalAllocated = OriCount;
	Ref->NumOpenSnapshots--;
}
#pragma endregion Arena

#pragma region Scratch
ForceInline p<ScratchHeaderEV>
ScratchHeaderEV::Get(p<void> data)
{
	p<sw> ptr = rcast< p<sw>>(data);

	while (ptr[-1] == sw(-1))
		ptr--;

	return rcast< p<ScratchHeaderEV>>(ptr) -1;
}

ForceInline void
ScratchHeaderEV::Fill(p<void> data, sw size)
{
	p<sw> ptr;

	Size = size;

	ptr = rcast< p<sw>>(this + 1);

	while (rcast< p<void>>(ptr) < data)
		dref ptr ++ = sw(-1);
}
#pragma endregion Scratch
#pragma endregion Implementation

}	// Memory
LAL_NamespaceEnd
#define HPP_LAL_Memory_Allocators
#endif

