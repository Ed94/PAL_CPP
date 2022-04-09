#pragma once

#include "LAL.Config.hpp"

// Much of this header is heavily ripped from EABase.

LAL_NamespaceStart

// Ed: Decided to go the route of sticking to latest version of C++ for this PAL.
// I like what the new standard is doing (minus the library implementers)
// and don't want to be force to do things in a more verbose manner.
#if __cplusplus < 202002L
	static_assert(false, "LAL: Requires a minimum of C++ 20");
#endif

// Cygwin
// This is a pseudo-platform which will be defined along with EA_PLATFORM_LINUX when
// using the Cygwin build environment.
#if defined(__CYGWIN__)
	#define Platform_CYGWIN 1
	#define Platform_Desktop 1
#endif

// MinGW
// This is a pseudo-platform which will be defined along with EA_PLATFORM_WINDOWS when
// using the MinGW Windows build environment.
#if defined(__MINGW32__) || defined(__MINGW64__)
	#define Platform_MINGW 1
	#define Platform_Desktop 1
#endif

#if defined(Platform_PS4) || defined(__ORBIS__)
	// PlayStation 4
	// Orbis was Sony's code-name for the platform, which is now obsolete.
	#if defined(Platform_PS4)
		#undef  Platform_PS4
	#endif
	#define Platform_PS4 1

	#define Platform_Name "PS4"
	#define System_Little_Endian 1
	#define Platform_Description "PS4 on x64"
	#define Platform_Console 1
	#define Platform_Sony 1
	#define Platfomr_POSIX 1
	// #define POSIX_THREADS_AVAILABLE 1  // POSIX threading API is available but discouraged.  Sony indicated use of the scePthreads* API is preferred. 
	#define Processor_X86_64 1
	#if defined(__GNUC__) || defined(__clang__)
		#define Asm_Style_ATT 1
	#endif

#elif defined(Platform_XboxONE) || defined(_DURANGO) || defined(_XBOX_ONE) || defined(_GAMING_XBOX)
	// XBox One
	// Durango was Microsoft's code-name for the platform, which is now obsolete.
	// Microsoft uses _DURANGO instead of some variation of _XBOX, though it's not natively defined by the compiler.
	// Capilano was an EA-specific code-name for the platform, which is now obsolete.
	#if defined(Platform_XboxONE)
		#undef  Platform_XboxONE
	#endif
	#define Platform_XboxONE 1

	#if !defined(_DURANGO)
		#define _DURANGO
	#endif
	#define Platform_Name "XBox One"
  //#define Processor_X86  Currently our policy is that we don't define this, even though x64 is something of a superset of x86.
	#define Processor_X86_64 1
	#define System_Little_Endian 1
	#define Platform_Description "XBox One on x64"
	#define Asm_Style_Intel 1
	#define Platform_Console 1
	#define Platform_Microsoft 1

	// WINAPI_FAMILY defines - mirrored from winapifamily.h
	#define WinAPI_Family_APP         1000
	#define WinAPI_Family_Desktop_APP 1002
	#define WinAPI_Family_TV_APP      1003
	#define WinAPI_Family_TV_TITLE    1004
	#define WinAPI_Family_GAMES       1006
	
	#if defined(WINAPI_FAMILY) 
		#include <winapifamily.h>
		#if defined(WINAPI_FAMILY_TV_TITLE) && WINAPI_FAMILY == WINAPI_FAMILY_TV_TITLE
			#define LAL_WINAPI_FAMILY	WinAPI_Family_TV_TITLE

		#elif defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			#define LAL_WINAPI_FAMILY	WinAPI_Family_Desktop_APP

		#elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
			#define LAL_WINAPI_FAMILY	WinAPI_Family_GAMES

		#else
			#error Unsupported WINAPI_FAMILY
		#endif
	#else
		#error WINAPI_FAMILY should always be defined on XBox.
	#endif

	// Macro to determine if a partition is enabled.
	#define WinAPI_Family_Partition(Partition)	(Partition)

	#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
		#define WinAPI_Partition_CORE     1
		#define WinAPI_Partition_DESKTOP  1
		#define WinAPI_Partition_APP      1
		#define WinAPI_Partition_PC_APP   0
		#define WinNPI_Partition_PHONE    0
		#define WinAPI_Partition_TV_APP   0
		#define WinAPI_Partition_TV_TITLE 0
		#define WinAPI_Partition_GAMES    0
	#elif EA_WINAPI_FAMILY == EA_WINAPI_FAMILY_TV_TITLE
		#define WinAPI_Partition_CORE     1
		#define WinAPI_Partition_DESKTOP  0
		#define WinAPI_Partition_APP      0
		#define WinAPI_Partition_PC_APP   0
		#define WinNPI_Partition_PHONE    0
		#define WinAPI_Partition_TV_APP   0
		#define WinAPI_Partition_TV_TITLE 1
		#define WinAPI_Partition_GAMES    0
	#elif EA_WINAPI_FAMILY == EA_WINAPI_FAMILY_GAMES
		#define WinAPI_Partition_CORE     1
		#define WinAPI_Partition_DESKTOP  0
		#define WinAPI_Partition_APP      0
		#define WinAPI_Partition_PC_APP   0
		#define WinNPI_Partition_PHONE    0
		#define WinAPI_Partition_TV_APP   0
		#define WinAPI_Partition_TV_TITLE 0
		#define WinAPI_Partition_GAMES    1
	#else
		#error Unsupported WINAPI_FAMILY
	#endif

	#if WinAPI_Family_Partition(WinAPI_Partition_GAMES)
		#define CS_UNDEFINED_STRING 			1
		#define CS_UNDEFINED_STRING 		1
	#endif

	#if WinAPI_Family_Partition(WinAPI_Partition_TV_TITLE)
		#define Platform_XboxOne_XDK 	1
	#endif

// Android (Google phone OS)
#elif defined(Platform_Andriod) || defined(__ANDROID__)
	#undef  Platform_Andriod
	#define Platform_Andriod 1
	#define Platform_Linux 1
	#define Platform_Unix 1
	#define Platform_POSIX 1
	#define Platform_Name "Android"
	#define Asm_Style_ATT 1
	#if defined(__arm__)
		#define ABI_Arm_Linux 1  // a.k.a. "ARM eabi"
		#define Processor_ARM32 1
		#define Platform_Description "Android on ARM"
	#elif defined(__aarch64__)
		#define Processor_ARM64 1
		#define Platform_Description "Android on ARM64"
	#elif defined(__i386__)
		#define Processor_X86 1
		#define Platform_Description "Android on x86"
	#elif defined(__x86_64)
		#define Processor_X86_64 1
		#define Platform_Description "Android on x64"
	#else
		#error Unknown processor
	#endif
	#if !defined(System_Big_Endian) && !defined(System_Little_Endian)
		#define System_Little_Endian 1
	#endif
	#define Platform_Mobile 1

#elif defined(__APPLE__) && __APPLE__
	#include <TargetConditionals.h>

	// Apple family of operating systems.
	#define Platform_Apple
	#define Platform_POSIX 1

	// Macintosh OSX
	// TARGET_OS_MAC is defined by the Metrowerks and older AppleC compilers.
	// Howerver, TARGET_OS_MAC is defined to be 1 in all cases.
	// __i386__ and __intel__ are defined by the GCC compiler.
	// __dest_os is defined by the Metrowerks compiler.
	// __MACH__ is defined by the Metrowerks and GCC compilers.
	// powerc and __powerc are defined by the Metrowerks and GCC compilers.
	#if defined(Platform_OSX) || defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
		#undef  Platform_OSX
		#define Platform_OSX 1
		#define Platfomr_Unix 1
		#define Platform_POSIX 1
	  //#define PLATFORM_BSD 1           We don't currently define this. OSX has some BSD history but a lot of the API is different.
		#define Platform_Name "OSX"
		#if defined(__i386__) || defined(__intel__)
			#define Processor_X86 1
			#define System_Little_Endian 1
			#define Platform_Description "OSX on x86"
		#elif defined(__x86_64) || defined(__amd64)
			#define Processor_X86 1
			#define System_Little_Endian 1
			#define Platform_Description "OSX on x64"
		#elif defined(__arm__)
			#define ABI_ARM_Apple 1
			#define Processor_ARM32 1
			#define System_Little_Endian 1
			#define Platform_Description "OSX on ARM"
		#elif defined(__aarch64__) || defined(__AARCH64)
			#define ABI_ARM64_Apple 1
			#define Processor_ARM64 1
			#define System_Little_Endian 1
			#define Platform_Description "OSX on ARM64"
		#elif defined(__POWERPC64__) || defined(__powerpc64__)
			#define Processor_POWERPC 1
			#define Processor_POWERPC_64 1
			#define System_Big_Endian 1
			#define Platform_Description "OSX on PowerPC 64"
		#elif defined(__POWERPC__) || defined(__powerpc__)
			#define Processor_POWERPC 1
			#define Processor_POWERPC_32 1
			#define System_Big_Endian 1
			#define Platform_Description "OSX on PowerPC"
		#else
			#error Unknown processor
		#endif
		#if defined(__GNUC__)
			#define Asm_Style_ATT 1
		#else
			#define Asm_Style_Motorola 1
		#endif
		#define Platform_Desktop 1
	#else
		#error Unknown Apple Platform
	#endif


// Linux
// __linux and __linux__ are defined by the GCC and Borland compiler.
// __i386__ and __intel__ are defined by the GCC compiler.
// __i386__ is defined by the Metrowerks compiler.
// _M_IX86 is defined by the Borland compiler.
// __sparc__ is defined by the GCC compiler.
// __powerpc__ is defined by the GCC compiler.
// __ARM_EABI__ is defined by GCC on an ARM v6l (Raspberry Pi 1)
// __ARM_ARCH_7A__ is defined by GCC on an ARM v7l (Raspberry Pi 2)
#elif defined(Platform_Linux) || (defined(__linux) || defined(__linux__))
	#undef  Platform_Linux
	#define Platform_Linux 1
	#define Platform_Unix 1
	#define Platform_POSIX 1
	#define Platform_Name "Linux"
	#if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
		#define Processor_X86 1
		#define System_Little_Endian 1
		#define Platform_Description "Linux on x86"
	#elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
		#define ABI_Arm_Linux 1
		#define Processor_ARM32 1
		#define Platform_Description "Linux on ARM 6/7 32-bits"
	#elif defined(__aarch64__) || defined(__AARCH64)
		#define Processor_ARM64 1
		#define Platform_Description "Linux on ARM64"
	#elif defined(__x86_64__)
		#define Processor_X86_64 1
		#define System_Little_Endian 1
		#define Platform_Description "Linux on x64"
	#elif defined(__powerpc64__)
		#define Processor_POWERPC 1
		#define Processor_POWERPC_64 1
		#define System_Big_Endian 1
		#define Platform_Description "Linux on PowerPC 64"
	#elif defined(__powerpc__)
		#define Processor_POWERPC 1
		#define Processor_POWERPC_32 1
		#define System_Big_Endian 1
		#define Platform_Description "Linux on PowerPC"
	#else
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if defined(__GNUC__)
		#define Asm_Style_ATT 1
	#endif
	#define Platform_Desktop 1


#elif defined(Platform_BSD) || (defined(__BSD__) || defined(__FreeBSD__))
	#undef  Platform_BSD
	#define Platform_BSD 1
	#define Platform_Unix 1
	#define Platform_POSIX 1     // BSD's posix complaince is not identical to Linux's
	#define Platform_Name "BSD Unix"
	#if defined(__i386__) || defined(__intel__)
		#define Processor_X86 1
		#define System_Little_Endian 1
		#define Platform_Description "BSD on x86"
	#elif defined(__x86_64__)
		#define Processor_X86_64 1
		#define System_Little_Endian 1
		#define Platform_Description "BSD on x64"
	#elif defined(__powerpc64__)
		#define Processor_POWERPC 1
		#define Processor_POWERPC_64 1
		#define System_Big_Endian 1
		#define Platform_Description "BSD on PowerPC 64"
	#elif defined(__powerpc__)
		#define Processor_POWERPC 1
		#define Processor_POWERPC_32 1
		#define System_Big_Endian 1
		#define Platform_Description "BSD on PowerPC"
	#else
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if !defined(Platform_FREEBSD) && defined(__FreeBSD__)
		#define Platform_FREEBSD 1 // This is a variation of BSD.
	#endif
	#if defined(__GNUC__)
		#define Asm_Style_ATT 1
	#endif
	#define Platform_Desktop 1

// Windows
// _WIN32 is defined by the VC++, Intel and GCC compilers.
// _WIN64 is defined by the VC++, Intel and GCC compilers.
// __WIN32__ is defined by the Borland compiler.
// __INTEL__ is defined by the Metrowerks compiler.
// _M_IX86, _M_AMD64 and _M_IA64 are defined by the VC++, Intel, and Borland compilers.
// _X86_, _AMD64_, and _IA64_ are defined by the Metrowerks compiler.
// _M_ARM is defined by the VC++ compiler.
#elif (defined(Platform_Windows) || (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))) && !defined(CS_UNDEFINED_STRING)
	#undef  Platform_Windows
	#define Platform_Windows 1
	#define Platform_Name "Windows"
	#ifdef _WIN64 // VC++ defines both _WIN32 and _WIN64 when compiling for Win64.
		#define Platform_Win64 1
	#else
		#define Platform_Win32 1
	#endif
	#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
		#define Processor_X86_64 1
		#define System_Little_Endian 1
		#define Platform_Description "Windows on x64"
	#elif defined(_M_IX86) || defined(_X86_)
		#define Processor_X86 1
		#define System_Little_Endian 1
		#define Platform_Description "Windows on X86"
	#elif defined(_M_IA64) || defined(_IA64_)
		#define Processor_IA64 1
		#define System_Little_Endian 1
		#define Platform_Description "Windows on IA-64"
	#elif defined(_M_ARM)
		#define EA_ABI_ARM_WINCE 1
		#define Processor_ARM32 1
		#define System_Little_Endian 1
		#define Platform_Description "Windows on ARM"
	#elif defined(_M_ARM64)
		#define Processor_ARM64 1
		#define System_Little_Endian 1
		#define Platform_Description "Windows on ARM64"
	#else //Possibly other Windows CE variants
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if defined(__GNUC__)
		#define EA_ASM_STYLE_ATT 1
	#elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__ICL)
		#define Asm_Style_Intel 1
	#endif
	#define Platform_Desktop 1
	#define Platform_Microsoft 1

	#if defined(_KERNEL_MODE)
		#define Platform_Windows_Kernel 1
	#endif

	// WINAPI_FAMILY defines to support Windows 8 Metro Apps - mirroring winapifamily.h in the Windows 8 SDK
	#define WinAPI_Family_APP         1000
	#define WinAPI_Family_DESKTOP_APP 1001
	#define WinAPI_Family_GAMES       1006

	#if defined(WINAPI_FAMILY)
		#if defined(_MSC_VER)
			#pragma warning(push, 0)
		#endif
		#include <winapifamily.h>
		#if defined(_MSC_VER)
			#pragma warning(pop)
		#endif
		#if defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			#define LAL_WinAPI_FAMILY			WinAPI_Family_DESKTOP_APP
		#elif defined(WINAPI_FAMILY_APP) && WINAPI_FAMILY == WINAPI_FAMILY_APP
			#define LAL_WinAPI_FAMILY			WinAPI_Family_APP
		#elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
			#define LAL_WinAPI_FAMILY			WinAPI_Family_GAMES
		#else
			#error Unsupported WINAPI_FAMILY
		#endif
	#else
		#define LAL_WinAPI_FAMILY WinAPI_Family_DESKTOP_APP
	#endif

	#define WinAPI_Partition_DESKTOP   1
	#define WinAPI_Partition_APP       1
	#define WinAPI_Partition_GAMES    (LAL_WinAPI_FAMILY == WinAPI_Family_GAMES)

	#define WinAPI_Family_Partition(Partition)   (Partition)

	// EA_PLATFORM_WINRT
	// This is a subset of Windows which is used for tablets and the "Metro" (restricted) Windows user interface.
	// WinRT doesn't doesn't have access to the Windows "desktop" API, but WinRT can nevertheless run on 
	// desktop computers in addition to tablets. The Windows Phone API is a subset of WinRT and is not included
	// in it due to it being only a part of the API.
	#if defined(__cplusplus_winrt)
		#define Platform_WinRT 1 
	#endif
#endif

#ifndef Processor_ARM
	#if defined(Processor_ARM32) || defined(Processor_ARM64) || defined(Processor_ARM7)
		#define Processor_ARM
	#endif
#endif

// EA_PLATFORM_PTR_SIZE
// Platform pointer size; same as sizeof(void*).
// This is not the same as sizeof(int), as int is usually 32 bits on
// even 64 bit platforms.
//
// _WIN64 is defined by Win64 compilers, such as VC++.
// _M_IA64 is defined by VC++ and Intel compilers for IA64 processors.
// __LP64__ is defined by HP compilers for the LP64 standard.
// _LP64 is defined by the GCC and Sun compilers for the LP64 standard.
// __ia64__ is defined by the GCC compiler for IA64 processors.
// __arch64__ is defined by the Sparc compiler for 64 bit processors.
// __mips64__ is defined by the GCC compiler for MIPS processors.
// __powerpc64__ is defined by the GCC compiler for PowerPC processors.
// __64BIT__ is defined by the AIX compiler for 64 bit processors.
// __sizeof_ptr is defined by the ARM compiler (armcc, armcpp).
//
#ifndef Platform_Ptr_Size
	#if defined(__WORDSIZE) // Defined by some variations of GCC.
		#define Platform_Ptr_Size ((__WORDSIZE) / 8)
	#elif defined(_WIN64) || defined(__LP64__) || defined(_LP64) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || \
	defined(__aarch64__) || defined(__mips64__) || defined(__64BIT__) || defined(__Ptr_Is_64) || defined(__powerpc64__) || \
	defined(__ppc64__) 
		#define Platform_Ptr_Size 8

	#elif defined(__CC_ARM) && (__sizeof_ptr == 8)
		#define Platform_Ptr_Size 8

	#else
		#define Platform_Ptr_Size 4
	#endif
#endif


// EA_PLATFORM_WORD_SIZE
// This defines the size of a machine word. This will be the same as
// the size of registers on the machine but not necessarily the same
// as the size of pointers on the machine. A number of 64 bit platforms
// have 64 bit registers but 32 bit pointers.
//
#ifndef Platform_Word_Size
	#define Platform_Word_Size Platform_Ptr_Size
#endif

// EA_PLATFORM_MIN_MALLOC_ALIGNMENT
// This defines the minimal alignment that the platform's malloc 
// implementation will return. This should be used when writing custom
// allocators to ensure that the alignment matches that of malloc
#ifndef Platform_Min_Malloc_Allignment
	#if defined(Platform_Apple)
		#define Platform_Min_Malloc_Allignment 16
	#elif defined(Platform_Andriod) && defined(Processor_ARM)
		#define Platform_Min_Malloc_Allignment 8
	#elif defined(Platform_Andriod) && defined(Processor_X86_64)
		#define Platform_Min_Malloc_Allignment 8
	#else
		#define Platform_Min_Malloc_Allignment (Platform_Ptr_Size * 2)
	#endif
#endif


// EA_MISALIGNED_SUPPORT_LEVEL
// Specifies if the processor can read and write built-in types that aren't
// naturally aligned.
//    0 - not supported. Likely causes an exception.
//    1 - supported but slow.
//    2 - supported and fast.
//
#ifndef Misaligned_Support_Level
	#if defined(Processor_X86_64)
		#define Misaligned_Support_Level 2
	#else
		#define Misaligned_Support_Level 0
	#endif
#endif

// Macro to determine if a Windows API partition is enabled. Always false on non Microsoft platforms.
#if !defined(WinAPI_Family_Partition)
	#define WinAPI_Family_Partition(Partition) (0)
#endif

// EA_CACHE_LINE_SIZE
// Specifies the cache line size broken down by compile target.
// This the expected best guess values for the targets that we can make at compilation time.
#ifndef Cache_Line_Size
	#if   defined(Processor_X86)      
		#define Cache_Line_Size 32    // This is the minimum possible value.
	#elif defined(Processor_X86_64)  
		#define Cache_Line_Size 64    // This is the minimum possible value
	#elif defined(Processor_ARM32)
		#define Cache_Line_Size 32    // This varies between implementations and is usually 32 or 64. 
	#elif defined(Processsor_ARM64)
		#define Cache_Line_Size 64    // Cache line Cortex-A8  (64 bytes) http://shervinemami.info/armAssembly.html however this remains to be mostly an assumption at this stage
	#elif (Platform_WordSize == 4)
		#define Cache_Line_Size 32    // This is the minimum possible value
	#else
		#define Cache_Line_Size 64    // This is the minimum possible value
	#endif
#endif

#if defined (__has_attribute)
#	define	Compiler_Has_Attribute(attribute_) __has_attribute(attribute_)
#else 
#	define	Compiler_Has_Attribute(attribute_) (0)
#endif

// Check if architecture is 32 or 64 bit.
#if defined(_WIN64)	|| defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || \
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
	static_assert(false, "LAL: Requires TypeTraits support");
#endif

LAL_NamespaceEnd

