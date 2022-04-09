#pragma once
#ifndef ALLOW_POLLUTION
static_assert(false, 
	"This file CANNOT be used outside of backend units. "
	"This file can POLLUTE translation global scope heavily."
);
#endif

#ifndef HPP_LAL

#include "LAL.Asserts.hpp"
#include "LAL.Bits.hpp"
#include "LAL.Casting.hpp"
#include "LAL.CString.hpp"
#include "LAL.Config.hpp"
#include "LAL.DArray.hpp"
#include "LAL.Data.hpp"
#include "LAL.Declarations.hpp"
#include "LAL.Limits.hpp"
#include "LAL.Math.hpp"
#include "LAL.Memory.hpp"
#include "LAL.Memory.Allocators.hpp"
#include "LAL.Metaprogramming.hpp"
#include "LAL.NumericLimits.hpp"
#include "LAL.Numerics.hpp"
#include "LAL.Platform.hpp"
#include "LAL.RefWrap.hpp"
#include "LAL.String.hpp"
#include "LAL.Tuple.hpp"
#include "LAL.Types.hpp"
#include "LAL.UTF.hpp"

#define HPP_LAL

#endif







