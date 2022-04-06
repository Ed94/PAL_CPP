#pragma once

#include "LAL.Config.hpp"


LAL_NamespaceStart
namespace CStr {

sw		Copy(p<char> destination, p<char const> source, sw length);
sw		Compare(p<char const> A, p<char const> B, sw length);

void	From_s64(s64 value, p<char> string, s32 base);
void	From_u64(u64 value, p<char> string, s32 base);

sw		Format	(p<char> string_out, sw length, p<char const> format, ...);
sw		Format_VA(p<char> string_out, sw maxLength, p<char const> format, va_list vargs);

s32		HexTo_s32(char c);

bool	IsAlpha(char c);
bool	IsAlphaNumeric(char c);
bool	IsDigit(char c);
bool	IsHex(char c);

sw		Length(p<char const> string);
sw		Length(p<char const> string, sw maxLength);

char	ToLower(char c);
void	ToLower(p<char> string);

char	ToUpper(char c);
void	ToUpper(p<char> string);

s64		To_s64(p<char const> string, p<p<char>> end, s32 base);
s64		To_s64(p<char const> string, sw length, p<p<char>> end, s32 base);

u64		To_u64(p<char const> string, p<p<char>> end, s32 base);
u64		To_u64(p<char const> string, sw length, p<p<char>> end, s32 base);


#pragma region Implementation
inline sw 
Copy(p<char> destination, p<char const> source, sw length)
{
	sw result = 0;
	//assert(destination != nullptr);

	if (source)
	{
		p<char const> start  = source;
		p<char>		  string = destination;

		while (length > 0 && dref source)
		{
			dref string = dref source;

			string++;
			source++;

			length--;
		}

		while (length > 0)
		{
			dref string = '\0';

			length--;
		}

		result = source - start;
	}

	return result;
}

inline sw
Compare(p<char const> A, p<char const> B, sw length)
{
	for (; length > 0; A++, B++, length--)
	{
		if (dref A != dref B)
			return ((A < B) ? -1 : +1);

		else if (dref A == '\0')
			return 0;
	}

	return 0;
}

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

inline sw 
Length(p<char const> string)
{
	if (string == nullptr)
		return 0;

	p<char const>
		offset = string;

	while (dref offset)
	{
		offset++;
	}

	return offset - string;
}

inline sw
Length(p<char const> string, sw maxLength) 
{
	p<char const> end = rcast<p<char const>>(Memory::Search(string, 0, maxLength));

	if (end) 
		return end - string;

	return maxLength;
}

inline sw 
Length(p<char> dest, p<char const> source, sw length) 
{
	sw result = 0;

	//ZPL_ASSERT_NOT_NULL(dest);

	if (source) 
	{
		p<char const>	source_start = source;
		p<char>			str = dest;
		
		while (length > 0 && *source) 
		{
			dref str = *source++;
			str++;
			length--;
		}

		while (length > 0)
		{
			dref str = '\0';
			str++;
			length--;
		}

		result = source - source_start;
	}

	return result;
}

inline char
ToLower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return 'a' + (c - 'A');

	return c;
}

inline void
ToLower(p<char> string)
{
	if (string == nullptr)
		return;

	while (dref string)
	{
		dref string = ToLower(dref string);
		string++;
	}
}

inline char
ToUpper(char c)
{
	if (c >= 'a' && c <= 'Z')
		return 'A' + (c - 'a');

	return c;
}

inline void
ToUpper(p<char> string)
{
	if (string == nullptr)
		return;

	while (dref string)
	{
		dref string = ToUpper(dref string);
		string++;
	}
}

#pragma endregion Implementaiton
} // CStr
LAL_NamespaceEnd

