#ifndef HPP_C_STL

//#define LAL_Bare_Runtime

#include "OSAL.Platform.hpp"

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

#define HPP_C_STL
#endif