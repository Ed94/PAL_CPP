#ifndef HPP_OSAL_Platform

#include "LAL.Types.hpp"
#include "API.Windows.hpp"

namespace OSAL
{
	enum class
	EOS : u32
	{
			Windows		=	0,
	#define M_OS_Windows	0
			Mac			=	1,
	#define M_OS_Mac		1
			Linux		=	2,
	#define M_OS_Linux		2
	};

	constexpr EOS
	OS = EOS(M_OS);

	constexpr bool	IsWindows =		OS == EOS::Windows;
	constexpr bool	IsMac     =		OS == EOS::Mac    ;
	constexpr bool	IsLinux   =		OS == EOS::Linux  ;

	struct OS_Version
	{
	#pragma region Layout
		u32 Major;
		u32 Minor;
		u32 Patch;

		u32 Build;
	#pragma endregion Layout

		struct Str const*
		Str() const;
	};


	// TODO : Str is currently undefined.
	struct Str const*
	Get_OSName();

	OS_Version const&
	Get_OSVersion();

	void Load_Platform();
}

#define HPP_OSAL_Platform
#endif