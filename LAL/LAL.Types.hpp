#pragma once
#include "LAL.Platform.hpp"

LAL_NamespaceStart

// Integers

// Flexible

using ui32 = unsigned int;
using si32 = signed   int;

static_assert(sizeof(ui32)  == sizeof(si32), "sizeof(ui32) != sizeof(si32)");

// Strict

using s8  = 			char;
using s16 = short		int;
using s32 = long		int;
using s64 = long long	int;

using u8  = unsigned				char;
using u16 = unsigned	short		int;
using u32 = unsigned	long		int;
using u64 = unsigned	long long	int;

static_assert(sizeof(s8)  == sizeof(s8) , "sizeof(u8)  != sizeof(s8)");
static_assert(sizeof(u16) == sizeof(s16), "sizeof(u16) != sizeof(s16)");
static_assert(sizeof(u32) == sizeof(s32), "sizeof(u32) != sizeof(s32)");
static_assert(sizeof(u64) == sizeof(s64), "sizeof(u64) != sizeof(s64)");

static_assert(sizeof(u8)  == 1, "sizeof(u8)  != 1");
static_assert(sizeof(u16) == 2, "sizeof(u16) != 2");
static_assert(sizeof(u32) == 4, "sizeof(u32) != 4");
static_assert(sizeof(u64) == 8, "sizeof(u64) != 8");

// Floats

#ifdef LAL_Float_16
using f16 = half;
#endif

using f32 = float;
using f64 = double;

// Chars

using ascii	 = char;
using wchar  = wchar_t;
using char8  = u8;
using char16 = u16;
using char32 = u32;

// Pointers

template<typename Type>
using p = Type*;

#ifdef Compiler_MSVC
typedef decltype(__nullptr)		nullptrType;
// TODO : Add other compiler implementations here.
#endif

// Addressing Op Wrappers

#define dref  *
#define ptrof &

template<class Type> Type*
addressof(Type& value) noexcept
{
	return 
		rcast<Type*>(& 
			ccast<char&>( 
				rcast<const volatile char&>(value)
			)
		);
}

// Hi-bit bools

using b16 = s16;
using b32 = s32;

LAL_NamespaceEnd