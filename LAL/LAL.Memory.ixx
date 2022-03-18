export module LAL.Memory;
#ifndef HPP_LAL_Memory

#include "LAL.C_STL.hpp"
#include "LAL.Types.hpp"

export 
template<class Type> 
ptr<void>
Format_ByFill(ptr<Type> memoryAddress, Type& fillValue, uw count)
{
	return memset(memoryAddress, count, fillValue);
}

export
template<class Type>
ptr<void>
Format_WithData(ptr<Type> memoryAddress, ptr<const Type> dataSource, uw count)
{
	return memcpy(memoryAddress, count, dataSource)
}

#define HPP_LAL_Memory
#endif