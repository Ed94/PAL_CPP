#pragma once

#include "LAL.Types.hpp"

namespace OSAL
{
	enum class
	EExitVal : u32
	{
		Success,
		Failure
	};

	// Engine application entrypoint.
	EExitVal EntryPoint();
}

