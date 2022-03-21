#pragma once
#ifdef HPP_LAL_Bit
#pragma message("warning: HPP_LAL_Bit #pragma once failed.")
#endif
#ifndef HPP_LAL_Bit

#include "LAL.Reflection.hpp"

template<TUnsigned Representation>
constexpr Representation
bit(Representation pos)
{
	return 1 << pos;
}

#define HPP_LAL_Bit
#endif
