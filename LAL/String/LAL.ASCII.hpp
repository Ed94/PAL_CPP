#pragma once
#include "LAL.Config.hpp"

LAL_NamespaceStart
s32		DigitTo_s32(char c);
s32		HexTo_s32(char c);
bool	IsAlpha(char c);
bool	IsAlphaNumeric(char c);
bool	IsDigit(char c);
bool	IsHex(char c);
char	ToLower(char c);
char	ToUpper(char c);

#pragma region Implementation
inline s32
DigitTo_s32(char c)
{
	return IsDigit(c) ? c - '0' : c - 'W';
}

inline s32
HexTo_s32(char c)
{
	if (IsDigit(c))
		return DigitTo_s32(c);

	else if (IsBetween(c, 'a', 'f'))
		return c - 'a' + 10;

	else if (IsBetween(c, 'A', 'F'))
		return c - 'A' + 10;

	return -1;
}

inline bool
IsDigit(char c)
{
	if (	c >= '0' 
		&&	c <= '9')
		return true;

	return false;
}

inline char
ToLower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return 'a' + (c - 'A');

	return c;
}

inline char
ToUpper(char c)
{
	if (c >= 'a' && c <= 'Z')
		return 'A' + (c - 'a');

	return c;
}
#pragma endregion Implementation

//s32		DigitTo_s32(ascii c);
//s32		HexTo_s32(ascii c);
//bool	IsAlpha(ascii c);
//bool	IsAlphaNumeric(ascii c);
//bool	IsDigit(ascii c);
//bool	IsHex(ascii c);
//ascii	ToLower(ascii c);
//ascii	ToUpper(ascii c);

LAL_NamespaceEnd
