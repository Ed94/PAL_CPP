#pragma once

#include "LAL.Config.hpp"
#include "LAL.Platform.hpp"
#include "LAL.Types.hpp"

LAL_NamespaceStart

#ifdef Compiler_MSVC
#	include <ymath.h>
#endif

#pragma region Integers
constexpr s8 s8_Min = (-0x7F - 1);
constexpr s8 s8_Max = 0x7F;
constexpr u8 u8_Max = 0xFFu;

constexpr s16 s16_Min = (-0x7FFF - 1);
constexpr s16 s16_Max = 0x7FFFF;
constexpr u16 u16_Max = 0xFFFFu;

constexpr s32 s32_Min = (-0x7FFFFFFF - 1);
constexpr s32 s32_Max = 0x7FFFFFFF;
constexpr u32 u32_Max = 0xFFFFFFFFu;

constexpr s64 s64_Min = (-0x7FFFFFFFFFFFFFFFll - 1);
constexpr s64 s64_Max = 0x7FFFFFFFFFFFFFFFll;
constexpr u64 u64_Max = 0xFFFFFFFFFFFFFFFFull;
#pragma endregion Integers

#pragma region Floats
#if (defined(Compiler_GCC) || defined(Compiler_Clang)) && defined(__FLT_MIN__)
	constexpr f32 f32_Min		= __FLT_MIN__;
	constexpr f32 f32_MinNeg    = f32_Min * -1.0f;
	constexpr f32 f32_Max		= __FLT_MAX__;
	constexpr f32 f32_MaxNeg    = f32_Max * -1.0f;
	constexpr f32 f32_Epsilon	= __FLT_EPSILON__;
	constexpr f32 f32_Infinity  = __builtin_huge_valf();
	constexpr f32 f32_QNaN		= __builtin_nanf("");
	constexpr f32 f32_SNaN		= __builtin_nansf("");
	constexpr f32 f32_DNormMin  = __FLT_DENORM_MIN__;
#elif (defined(Compiler_MSVC))
	constexpr f32 f32_Min		= 1.17549435e-38f;
	constexpr f32 F32_MinNeg	= f32_Min * -1.0f;
	constexpr f32 f32_Max		= 3.40282347e+38f;
	constexpr f32 f32_MaxNeg	= f32_Max * -1.0f;
	constexpr f32 f32_Epsilon	= 1.192092896e-07f;
#	define		  f32_Infinity    _FInf._Float
#	define		  f32_QNaN		  _FNan._Float
#	define		  f32_SNaN		  _FSnan._Float
#	define		  f32_DNormMin	  _FDenorm._Float
#endif

#if (defined(Compiler_GCC) || defined(Compiler_Clang)) && defined(__DBL_MIN__)
	constexpr f64 f64_Min		= __DBL_MIN__;
	constexpr f64 f64_MinNeg    = f64_Min * -1.0f;
	constexpr f64 f64_Max		= __DBL_MAX__;
	constexpr f64 f64_MaxNeg    = f64_Max * -1.0f;
	constexpr f64 f64_Epsilon	= __DBL_EPSILON__;
	constexpr f64 f64_Infinity  = __builtin_huge_valf();
	constexpr f64 f64_QNaN		= __builtin_nanf("");
	constexpr f64 f64_SNaN		= __builtin_nansf("");
	constexpr f64 f64_DNormMin  = __DBL_DENORM_MIN__;
#elif (defined(Compiler_MSVC))
	constexpr f64 f64_Min		= 2.2250738585072014e-308;
	constexpr f64 f64_MinNeg	= f64_Min * -1.0;
	constexpr f64 f64_Max		= 1.7976931348623157e+308;
	constexpr f64 f64_MaxNeg    = f64_Max * -1.0;
	constexpr f64 f64_Epsilon   = 2.2204460492503131e-01;
#	define		  f64_Infinity    _Inf._Double
#	define		  f64_QNaN		  _Nan._Double
#	define		  f64_SNaN		  _Snan._Double
#	define		  f64_DNormMin	  _Denorm._Double
#endif

constexpr f32 DeciAccuracy  = 0.1f;
constexpr f32 CentiAccuracy = 0.01f;
constexpr f32 MilliAccuracy = 0.001f;
constexpr f32 MicroAccuracy = 0.00001f;
constexpr f64 NanoAccuracy  = 0.00000001f;
#pragma endregion Floats

LAL_NamespaceEnd
