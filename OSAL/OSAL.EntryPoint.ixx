export module OSAL.EntryPoint;
#ifndef HPP_OSAL_EntryPoint

#include "LAL.Types.hpp"

export
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
#define HPP_OSAL_EntryPoint
#endif