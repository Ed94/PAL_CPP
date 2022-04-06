/*	Explicit Specifiers
*/
#pragma once

// #include "Meta/Config/CoreDev_Config.hpp"
#include "LAL.Platform.hpp"

LAL_NamespaceStart

// Analysis

#if defined(Compiler_MSVC)
#    define unused_param(x) (__pragma(warning(suppress : 4100))(x))

#elif defined(Compiler_GCC)
#    define unused_param(x) __attribute__((__unused__))(x)

#else
#    define unused_param(x) ((void)(zpl_size_of(x)))
#endif

// Linkage :

// Creates a static duration variable only accessible to the file (translation unit). (Global scope/File scope only)
#define InternLink \
static

// Creates a static duration variable accessible to any file (translation unit) or linkage. (Global/File scope Only)
#define ExternLink \
extern

// Inlines

// Alias for an inline variable that is supposed to have a constant value, but exist in multiple definitions.
#define multiDefs \
inline

// Alias for a constexpr intended to be used as an inline expansion.
#define constInline 

#ifdef						LAL_ForceInline_EnforceDiscretion

	// Standard force inline define. See: https://en.wikipedia.org/wiki/Inline_function
	#ifdef Compiler_MSVC
							#define ForceInline __forceinline
	#elif defined(Compiler_GCC)
							#define ForceInline inline __attribute__((__always_inline__))
	#elif defined(Compiler_Clang)
	#if __has_attribute(__always_inline__)
							#define ForceInline inline __attribute__((__always_inline__))
	#else
							#define ForceInline inline
	#endif
	#else
							#define ForceInline inline
	#endif
#else
							// Using compiler discretion.
							#define ForceInline inline
#endif

#ifdef						LAL_NeverInline_EnforceDiscretion
#	if Compiler_Has_Attribute(noinline)
#		if Compiler_GCC
#								define NeverInline __attribute__((__noinline__))
#		elif Compiler_MSVC
#								define _declspec(noinline)
#		endif
#	else
#								define NeverInline
#	endif
#else
#								define NeverInline
#endif


// Used in context with a namespace declaration to signify type_as the latest. (If using a versioning scheme for namespacing.
#define latest_Implementation \
inline

LAL_NamespaceEnd
