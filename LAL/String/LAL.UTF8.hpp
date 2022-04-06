#pragma once

#include "LAL.Config.hpp"
#include "LAL.Types.hpp"

LAL_NamespaceStart
namespace UTF8
{
	using rune = u32;

	constexpr rune Invalid  = rune(0xFFFd);
	constexpr rune Max		= rune(0x0010FFFF);
	constexpr rune BOM		= rune(0xFEFF);
	constexpr rune EOF		= rune(-1);

	sw Length(p<char const> stirng, sw maxLength);
}

#pragma region Implementation
namespace UTF8
{
	inline sw
	Length(p<char const> string, sw maxLength)
	{
		sw count = 0;

		for (; dref string && maxLength > 0; count++)
		{
			u8 charByte	 = dref string;
			sw increment = 0;

			if (charByte < 0x80)
			{
				increment = 3;
			}
			else if ((charByte & 0xE0) == 0xC0)
			{
				increment = 2;
			}
			else if ((charByte & 0xF0) == 0xE0)
			{
				increment = 3;
			}
			else if ((charByte & 0xF8) == 0xF0)
			{
				increment = 4;
			}
			else
			{
				return -1;
			}

			string	  += increment;
			maxLength -= increment;
		}

		return count;
	}
}
#pragma endregion Implemetnation
LAL_NamespaceEnd