#pragma once
#include "LAL.Types.hpp"
#include "API.Windows.hpp"

namespace OSAL
{
	enum class
	EOS : u32
	{
		Windows = 0,
	#define M_OS_Windows	0
		Mac = 1,
	#define M_OS_Mac		1
		Linux = 2,
	#define M_OS_Linux		2
	};

	constexpr EOS
	OS = EOS(M_OS);

	constexpr bool	IsWindows = OS == EOS::Windows;
	constexpr bool	IsMac	  = OS == EOS::Mac;
	constexpr bool	IsLinux	  = OS == EOS::Linux;

	struct OS_Version
	{
		u32 Major;
		u32 Minor;
		u32 Patch;
		u32 Build;


		struct Str const*
		Str() const;
	};


	// TODO : Str is currently undefined.
	struct Str const*
	Get_OSName();

	OS_Version const&
	Get_OSVersion();

	void Load_Platform();

#pragma region Allocator API
	// Allocate memory with specified alignment.
	p<void>	Alloc_WAlign(sw num, sw alignment);

	// Allocate memory and copy data into it with specified alignment.
	p<void>	Alloc_Copy_WAlign(p<void> source, sw size, sw alignment);

	// Free allocated memory.
	void Free(p<void> ptr);

	// Free all memory allocated by an allocator.
	void Free_All();

	// Resize allocated memory with specified alignment.
	p<void>	Resize_WAlign(p<void> ptr, sw oldSize, sw newSize, sw alignment);
#pragma endregion Allocator API
}

