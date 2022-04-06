#include "OSAL.Platform.hpp"

#define ALLOW_POLLUTION
#include "API.Infoware.hpp"

namespace OSAL
{
#pragma region Static Data
	// TODO : Str is currently undefined.
	std::string		OS_Name;
	OS_Version 		OS_Ver;
#pragma endregion Static Data

	void Load_Platform()
	{
		namespace system = iware::system;

		system::OS_info_t 
		osInfo = system::OS_info();

		// TODO: Conversion should happen here (With proper string type).
		OS_Name = osInfo.full_name;

		OS_Ver.Major = osInfo.major       ;
		OS_Ver.Minor = osInfo.minor       ;
		OS_Ver.Patch = osInfo.patch       ;
		OS_Ver.Build = osInfo.build_number;

		//Log("OS: " + OS_Name + " Version: " + OS_Ver.Str());
	}

	// TODO : Str is currently undefined.
	struct Str { char* Data; } const*
	Get_OSName()
	{
		return { nullptr };
	}

	OS_Version const&
	Get_OSVersion ()
	{
		return OS_Ver;
	}
}
