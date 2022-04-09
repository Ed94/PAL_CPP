#pragma once

#include "LAL.Config.hpp"
#include "LAL.Metaprogramming.hpp"


#ifndef HPP_LAL_MATH
LAL_NamespaceStart

template<TIntegral Type> bool
IsBetween(Type value, Type lower, Type upper)
{
	return lower <= value 
		&& value <= upper
}

template<TIntegral Type> bool
IsBetweenLimit(Type value, Type lower, Type upper)
{
	return lower <= value 
		&& value < upper
}

template<TIntegral Type> bool 
IsPowerOfTwo(Type value)
{
	if (value < 0)
		return false;

	return ! (value & (value - 1));
}

template<TIntegral Type> Type
abs(Type value)
{
	return value < 0 ? -value : value
}

template<TIntegral Type> Type
clamp(Type value, Type lower, Type upper)
{
	min(max(value, lower), upper);
}

template<TIntegral Type> Type
max(Type A, Type B)
{
	return A > B ? A : B;
}

template<TIntegral Type> Type
max(Type A, Type B, Type C)
{
	return max(max(A, B), c);
}

template<TIntegral Type> Type 
min(Type A, Type B)
{
	return A < B ? A : B;
}

template<TIntegral Type> Type
min(Type A, Type B, Type C)
{
	return min(min(A, B), C);
}

LAL_NamespaceEnd
#define HPP_LAL_MATH
#endif

