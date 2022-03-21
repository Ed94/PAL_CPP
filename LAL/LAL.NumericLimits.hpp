/*	Based off the EASTL : numeric_limits.h
*/
#pragma once

#include "LAL.Types.hpp"

LAL_NamespaceStart

enum class
EFloat_Rounding : u32
{
	Indeterminate	= 0,
	TowardZero		= 1,
	ToNearest		= 2,
};

LAL_NamespaceEnd

