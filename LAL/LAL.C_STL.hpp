#pragma once
#ifndef ALLOW_POLLUTION
static_assert(false, 
	"This file CANNOT be used outside of backend units. "
	"This file CAN POLLUTE global scope heavily."
);
#endif

#include "API.Windows.hpp"

//#define LAL_Bare_Runtime


#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Weverything"
#endif
//	C Libraries
#	ifndef LAL_Bare_Runtime
#		include <cassert>
#		include <cmath>
#		include <cstdio>
#		include <cstdlib>
#	endif
#		include <cstdarg>
#		include <cstddef>
#		include <cstdint>
#		include <intrin.h>
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

