#pragma once

#include "LAL.Config.hpp"
#include "LAL.Reflection.hpp"

#ifndef HPP_LAL_MATH
LAL_NamespaceStart

template<TIntegral Type> bool 
IsPowerOfTwo(Type value)
{
	if (value < 0)
		return false;

	return ! (value & (value - 1));
}

template<TIntegral Type> Type
max(Type A, Type B)
{
	return A > B ? A : B;
}

template<TIntegral Type> Type 
min(Type A, Type B)
{
	return A < B ? A : B;
}

LAL_NamespaceEnd
#define HPP_LAL_MATH
#endif

#include <type_traits>
