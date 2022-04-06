/*	Based off the EASTL : numeric_limits.h
*/
#pragma once

#include "LAL.Limits.hpp"

LAL_NamespaceStart

enum class
EFloat_Rounding : u32
{
	Indeterminate	= -1,
	TowardZero		= 1,
	ToNearest		= 2,
};

enum class
EFloat_DenormalizedStyle
{
	Indeterminate	= -1,
	Abscent			= 0,
	Present			= 1
};

struct NumLimitsBase
{
	// True if the type has an explicit specialization defined in the template class; false if not. 
	static constexpr bool 
	IsSpecialized = false;

	// The number of decimal digits that can be represented by T.
	// Equivalent to FLT_DIG, DBL_DIG or LDBL_DIG for floating types.
	static constexpr s32 
	Digits10 = 0;

	// The number of decimal digits required to make sure that two distinct values of the type have distinct decimal representations.
	static constexpr int 
	MaxDigits10 = 0;

	// True if the type is signed.
	static constexpr bool
	IsSigned = false;

	// True if the type is integral.
	static constexpr bool 
	IsInteger = false;

	// True if the type uses an exact representation. All integral types are
	// exact, but other types can be exact as well.
	static constexpr bool 
	IsExact = false;

	// Integer types: the base of the representation. Always 2 for integers.
	// Floating types: the base of the exponent representation. Always FLT_RADIX (typically 2) for float.
	static constexpr int 
	Radix = 0;

	// The minimum integral radix-based exponent representable by the type.
	static constexpr int 
	MinExponent = 0;

	// The minimum integral base 10 exponent representable by the type.
	static constexpr int 
	MinExponent10 = 0;

	// The maximum integral radix-based exponent representable by the type.
	static constexpr int 
	MaxExponent = 0;

	// The maximum integral base 10 exponent representable by the type.
	static constexpr int 
	MaxExponent10 = 0;

	// True if the type has a representation for positive infinity.
	static constexpr bool 
	HasInfinity = false;

	//  True if the type has a representation for a quiet (non-signaling) NaN.
	static constexpr bool 
	HasQNaN = false;

	// True if the type has a representation for a signaling NaN.
	static constexpr bool 
	HasSNaN = false;

	// An enumeration which identifies denormalization behavior.
	// In practice the application can change this at runtime via hardware-specific commands.
	static constexpr EFloat_DenormalizedStyle 
	HasDenorm = EFloat_DenormalizedStyle::Abscent;

	// True if the loss of accuracy is detected as a denormalization loss.
	// Typically false for integer types and true for floating point types.
	static constexpr bool 
	HasDNormLoss = false;

	// True if the type has a bounded set of representable values. Typically true for 
	// all built-in numerial types (integer and floating point).
	static constexpr bool 
	IsBounded = false;

	// True if the type has a modulo representation (if it's possible to add two
	// positive numbers and have a result that wraps around to a third number
	// that is less. Typically true for integers and false for floating types.
	static constexpr bool 
	IsModulo = false;

	// True if trapping (arithmetic exception generation) is implemented for this type.
	// Typically true for integer types (div by zero) and false for floating point types,
	// though in practice the application may be able to change floating point to trap at runtime.
	static constexpr bool 
	Traps = false;

	// True if tiny-ness is detected before rounding.
	static constexpr bool 
	TinynessBefore = false;

	// An enumeration which identifies default rounding behavior.
	// In practice the application can change this at runtime via hardware-specific commands.
	static constexpr EFloat_Rounding 
	Roundtyle = EFloat_Rounding::TowardZero;

	// True if the type is floating point and follows the IEC 559 standard (IEEE 754).
	// In practice the application or OS can change this at runtime via hardware-specific commands or via compiler optimizations.
	static constexpr bool
	IsIEC559 = false;
};

template<class Type_>
struct NumLimits : public NumLimitsBase
{
	using Type = Type_;

	static Type		Min()			{ return Type(0); };
	static Type		Max()			{ return Type(0); };
	static Type		Lowest()		{ return Min(); }
	static Type		Epsilon()		{ return Type(0); }
	static Type    	RoundError()	{ return Type(0); }
	static Type		denorm_min()	{ return Type(0); }
	static Type		infinity()		{ return Type(0); }
	static Type		quiet_NaN()		{ return Type(0); }
	static Type		signaling_NaN() { return Type(0); }
};

template<class Type> struct NumLimits<const			 Type> : public NumLimits<Type> {};
template<class Type> struct NumLimits<		volatile Type> : public NumLimits<Type> {};
template<class Type> struct NumLimits<const volatile Type> : public NumLimits<Type> {};



LAL_NamespaceEnd

