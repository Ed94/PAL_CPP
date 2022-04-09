#pragma once
#include "LAL.Config.hpp"
#include "LAL.Metaprogramming.hpp"
#include "LAL.Types.hpp"

LAL_NamespaceStart

/*
Grapheme / Glyth character
A character type that supports any character used by LAL/PAL.
*/

template<TAscii Type>
struct Char_Base<Type>
{
	static constexpr uw Width = sizeof(Type);

	static constexpr Type CarriageReturn	= 0xD;
	static constexpr Type FormFeed			= 0xC;
	static constexpr Type HorizontalTab		= 0x8;
	static constexpr Type LineFeed			= 0xA;
	static constexpr Type VerticalTab		= 0xB;
};

template<TWideChar Type>
struct Char_Base {
	static constexpr uw Width = sizeof(Type);
#pragma region ASCII
	static constexpr Type CarriageReturn	= 0xD;
	static constexpr Type FormFeed			= 0xC;
	static constexpr Type HorizontalTab		= 0x8;
	static constexpr Type LineFeed			= 0xA;
	static constexpr Type VerticalTab		= 0xB;
#pragma endregion ASCII

#pragma region Unicode
	static constexpr Type LineSeparator		 = 0x2028;
	static constexpr Type NextLine			 = 0x85;
	static constexpr Type ParagraphSeparator = 0x2029;
#pragma endregion Unicode	
};

template<TChar Type>
struct Char : Char_Base<Type> {

static bool 
IsLinebreak(Type Char)
{
	const u32 code = Char;
	if constexpr (Width > 1)
	{
		return 
			(code - LineFeed <= u32(CarriageReturn - LineFeed)) 
		|	(code == NextLine) 
		|	(code == LineSeparator) 
		|	(code == ParagraphSeparator)
		;
	}
	else
	{
		return (code - LineFeed <= u32(CarriageReturn - LineFeed));
	}
}

}; // Char

LAL_NamespaceEnd
