export module OSAL;
#ifndef HPP_OSAL

namespace OSAL
{
	void Load();
	void Unload();

	void PollEvents();
}

#define HPP_OSAL
#endif