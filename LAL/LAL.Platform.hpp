#pragma once

#include "LAL.Config.hpp"

LAL_NamespaceStart

#if defined (__has_attribute)
#	define	Compiler_Has_Attribute(attribute_) __has_attribute(attribute_)
#else 
#	define	Compiler_Has_Attribute(attribute_) (0)
#endif

// Check if architecture is 32 or 64 bit.
#if defined(_WIN64)	|| defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) ||       \
	defined(__ppc64__) || defined(__aarch64__)
#		ifndef CPU_Arch_WordSize_64
#				define CPU_Arch_WordSize_64 1
#		endif
#else
#		ifndef CPU_Arch_WordSize_32
				define CPU_Arch_WordSize_32 1
#		endif
#endif

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

#ifndef Compiler_Has_Feature
#	if defined(Compiler_Clang)
#		define Compiler_Has_Feature(x) __has_feature(x)
#	else
#		define Compiler_Has_Feature(x) 0
#	endif
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

	// Compilers that always implement wchar_t as native include:
	//     COMEAU, new SN, and other EDG-based compilers.
	//     GCC
	//     Borland
	//     SunPro
	//     IBM Visual Age
#if defined(Compiler_MSVC) || (defined(Compiler_Clang) && (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)))
#		ifndef _NATIVE_WCHAR_T_DEFINED
#				define wchar_t_NonNative 1
#		endif
#elif defined(__EDG_VERSION__) && (!defined(_WCHAR_T) && (__EDG_VERSION__ < 400)) 
	// EDG prior to v4 uses _WCHAR_T to indicate if wchar_t is native. v4+ may define something else, but we're not currently aware of it.
#				define wchar_t_NonNative 1
#else 
//#				define wchar_t_NonNative 0
#endif

///////////////////////////////////////////////////////////////////////////////
// Compiler_Intrinsic_TypeTraits_Supported
//
// Defined as 0 or 1; default is based on auto-detection.
// Specifies whether the compiler provides built-in compiler type trait support
// (e.g. __is_abstract()). Does not specify any details about which traits
// are available or what their standards-compliance is. Nevertheless this is a
// useful macro identifier for our type traits implementation.
//
#if defined(Compiler_MSVC) && (_MSC_VER >= 1500)  // VS2008 or later
#	pragma warning(push, 0)
#		include <yvals.h>
#	pragma warning(pop)

#	if ((defined(_HAS_TR1) && _HAS_TR1) || _MSC_VER >= 1700)  // VS2012 (1700) and later has built-in type traits support.
#			define Compiler_Intrinsic_TypeTraits_Supported 1
#	else
#			define Compiler_Intrinsic_TypeTraits_Supported 0
#	endif

#elif defined(Compiler_Clang) && defined(__APPLE__) && defined(_CXXCONFIG) // Apple clang but with GCC's libstdc++.
#			define Compiler_Intrinsic_TypeTraits_Supported 0

#elif defined(Compiler_Clang)
#			define Compiler_Intrinsic_TypeTraits_Supported 1

#elif defined(Compiler_GCC) && !defined(__GCCXML__)
#			define Compiler_Intrinsic_TypeTraits_Supported 1

#elif defined(__MSL_CPP__) && (__MSL_CPP__ >= 0x8000) // CodeWarrior compiler.
#			define Compiler_Intrinsic_TypeTraits_Supported 1

#else
#			define Compiler_Intrinsic_TypeTraits_Supported 0

#endif

#if !Compiler_Intrinsic_TypeTraits_Supported
#	pragma message("Your using a compiler that doesn't support reflection intrinsicts.")
#	pragma message("The type trait code implementation is only meant for educational purposes and is not mean to for actual use.")
#endif

LAL_NamespaceEnd

