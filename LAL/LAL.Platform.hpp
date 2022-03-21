#pragma once

#include "LAL.Config.hpp"

LAL_NamespaceStart

/* Platform compiler */
enum class ECompiler
{
	MSVC,
	GCC,
	Clang,
	MinGW,
	TinyC
};

#if defined(_MSC_VER)
#	define Compiler_MSVC 1
	constexpr ECompiler Compiler = ECompiler::MSVC;

#elif defined(__GNUC__)
#	define Compiler_GCC 1
	constexpr ECompiler Compiler = ECompiler::GCC;

#elif defined(__clang__)
#    define Compiler_Clang 1
	constexpr ECompiler Compiler = ECompiler::Clang;
#elif defined(__MINGW32__)
#    define Compiler_MinGW 1
	constexpr ECompiler Compiler = ECompiler::MinGW;

#elif defined(__TINYC__)
#    define Compiler_TinyC 1
	constexpr ECompiler Compiler = ECompiler::TinyC;

#else
#    error Unknown compiler
#endif

#if defined(Compiler_GCC) && defined(__x86_64__)
#		define Compiler_IntMax_Size 16  // intmax_t is __int128_t (GCC extension) and is 16 bytes.
#else
#		define Compiler_IntMax_Size 8   // intmax_t is int64_t and is 8 bytes.
#endif

#if defined (__SIZEOF_INT128__) || (defined Compiler_IntMax_Size) && (Compiler_IntMax_Size >= 16)
#		define M_Int128_Supported 1
		constexpr bool Int128_Supported = true;
#else
#		define M_Int128_Supported 0
		constexpr bool Int128_Supported = false;
#endif

#ifndef EA_WCHAR_T_NON_NATIVE
	// Compilers that always implement wchar_t as native include:
	//     COMEAU, new SN, and other EDG-based compilers.
	//     GCC
	//     Borland
	//     SunPro
	//     IBM Visual Age
#	if defined(Compiler_MSVC) || (defined(Compiler_Clang) && (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)))
#		ifndef _NATIVE_WCHAR_T_DEFINED
#				define wchar_t_NonNative 1
#		endif
#	elif defined(__EDG_VERSION__) && (!defined(_WCHAR_T) && (__EDG_VERSION__ < 400)) 
	// EDG prior to v4 uses _WCHAR_T to indicate if wchar_t is native. v4+ may define something else, but we're not currently aware of it.
#				define wchar_t_NonNative 1
#	else 
//#				define wchar_t_NonNative 0
#	endif
#endif

LAL_NamespaceEnd
