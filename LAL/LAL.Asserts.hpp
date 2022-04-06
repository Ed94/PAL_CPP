#pragma once

#include "LAL.Config.hpp"
#include "LAL.Platform.hpp"


void Debug_Trap()
{
	// Does nothing if there isn't a supported trap routine for the debugger.
#	ifdef Compiler_MSVC
	{
#		ifdef constexpr (_MSC_VER < 1300)
		{
			_asm int 3;
		}
#		else
		{
			__debugbreak();
		}
#		endif
	}
#	else
	{
		__builtin_trap();
	}
#	endif
}

