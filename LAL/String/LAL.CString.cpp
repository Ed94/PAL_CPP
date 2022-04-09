#include "LAL.CString.hpp"

#define ALLOW_POLLUTION
#include "LAL.C_STL.hpp"

namespace CStr {

enum class
EFormat : u32
{
	Minus	= bit(0),
	Plus	= bit(1),
	Alt		= bit(2),
	Space	= bit(3),
	Zero	= bit(4),

	Char	= bit(5),
	Short	= bit(6),
	Int		= bit(7),
	Long	= bit(8),
	LLong	= bit(9),
	Size	= bit(10),
	IntPtr	= bit(11),

	Unsigned	= bit(12),
	Lower		= bit(13),
	Upper		= bit(14),
	Width		= bit(15),

	Done = bit(30),

	Ints = Char | Short | Int | Long | LLong | Size | IntPtr,

	TTrait_Enum_AreBitFlags
};

//u32 operator | (EFormat a, EFormat b) { return (u32)a | (u32)b; }

struct FormatInfo
{
	s32 Base;
	s32 Flags;
	s32 Width;
	s32 Precision;
};

sw Format_Char(p<char> string_out, sw maxLength, p<FormatInfo> Info, char Char);
sw Format_Str(p<char> string_out, sw maxLength, p<FormatInfo> info, p<char const> string_in);
sw Format_Str(p<char> string_out, sw maxLength, p<FormatInfo> info, p<char const> string_in, sw length);
sw Format_CharRepeated(p<char> string_out, sw maxLength, p<FormatInfo> info, char arg);

NeverInline	sw
Format_VA(p<char> string_out, sw maxLength, p<char const> format, va_list vargs)
{
#define current		dref format

	p<char const> start = string_out;

	sw 
	remaining = maxLength, 
	result;

	while (current)
	{
		sw length = 0;

		FormatInfo
		info			= { 0 };
		info.Precision	= -1;

		while (		current 
				&&	current != '%'
				&&	remaining)
		{
			dref string_out++ = current++;
		}

		if (current == '%')
		{
			do
			{
				switch (dref ++ format)
				{
					case '-':
						info.Flags |= EFormat::Minus;
					break;

					case '+':
						info.Flags |= EFormat::Plus;
					break;

					case '#':
						info.Flags |= EFormat::Alt;
					break;

					case ' ':
						info.Flags |= EFormat::Space;
					break;

					case '0':
						info.Flags |= EFormat::Zero | EFormat::Width;
					break;

					default:
						info.Flags |= EFormat::Done;
					break;
				}
			}
			while (! (info.Flags & EFormat::Done));
		}

		// Optional Width
		if (current == '*')
		{
			int width = va_arg(vargs, int);

			if (width < 0)
			{
				info.Flags |= EFormat::Minus;
				info.Width = -width;
			}
			else
			{
				info.Width = width;
			}

			info.Flags |= EFormat::Width;
			format++;
		}
		else
		{
			info.Width = (u32)	To_s64(format, p< p<char>>(ptrof format), 10);

			if (info.Width != 0)
				info.Flags |= EFormat::Width;
		}

		// Optional Precision
		if (current == '.')
		{
			format++;

			if (current == '*')
			{
				info.Precision = va_arg(vargs, int);
				format++;
			}
			else
			{
				info.Precision = (s32) To_s64(format, p< p<char>>(ptrof format), 10);
			}

			info.Flags &= EFormat::Zero;
		}

		switch (current++)
		{
			case 'h':
				if (current == 'h')
				{
					info.Flags |= EFormat::Char;
					format++;
				}
				else
				{
					info.Flags |= EFormat::Long;
				}
			break;

			case 'l':
				if (current == 'l')
				{
					info.Flags |= EFormat::Minus;
					format++;
				}
				else
				{
					info.Flags |= EFormat::Long;
				}
			break;

			case 'z' :
				info.Flags |= EFormat::Unsigned;
			[[fallthrough]]
			case 't':
				info.Flags |= EFormat::Size;
			break;

			default: 
				format--;
			break;
		}

		switch (current)
		{
			
		}
	}

#undef current;
}

s64 To_s64(p<char const> string, p< p<char>> end, s32 base)
{
	sw	length = 0;
	s64 value  = 0;

	if (! base)
	{
		if (Length(string) > 2 && Compare(string, "0x", 2) == 0)
			base = 16;

		else
			base = 10;
	}

	length = Scan_s64(string, base, ptrof value);

	if (end)
		dref end = p<char>(string) + length;

	return value;
}

s64 To_s64(p<char const> string, sw length, p< p<char>> end, s32 base)
{
	s64 value = 0;

	if (! base)
	{
		if (length > 2 && Compare(string, "0x", 2) == 0)
			base = 16;

		else
			base = 10;
	}

	length = Scan_s64(string, base, ptrof value);

	if (end)
		dref end = p<char>(string) + length;

	return value;
}

// TODO: (zpl) Make Better
u64 To_u64(p<char const> string, p< p<char>> end, s32 base)
{
	sw	length = 0;
	u64 value  = 0;

	if (! base)
	{
		if (Length(string) > 2 && Compare(string, "0x", 2) == 0)
			base = 16;

		else
			base = 10;
	}

	length = Scan_u64(string, base, ptrof value);

	if (end)
		dref end = p<char>(string) + length;

	return value;
}

#pragma region Internal
InternLink sw
Scan_s64(p<char const> string, s32 base, p<s64> value)
{
	p<char const> start = string;

	s64 result		= 0;
	b32 negative	= false;

	if (dref string == '-')
	{
		negative = true;
		string++;
	}

	if (base == 16 && Compare(string, "0x", 2) == 0)
		string += 2;
		
	s64 v;
	if (base == 10)
	{
		for (;;)
		{
			if (IsDigit(dref string))
				v = dref string - '0';

			else
				break;

			result *= base;
			result += v;
			string++;
		}
	}
	else // Hex
	{
		for (;;)
		{
			if (IsHex(dref string))
				v = HexTo_s32(dref string);

			else
				break;

			result *= base;
			result += v;
			string++;
		}
	}

	if (value)
	{
		if (negative)
			result = -result;

		dref value = result;
	}

	return (string - start);
}

InternLink sw
Scan_u64(p<char const> string, s32 base, p<u64> value)
{
	p<char const> start = string;

	u64 result = 0;

	if (base == 16 && Compare(string, "0x", 2) == 0)
		string += 2;

	if (base == 10)
	{
		for (;;)
		{
			u64 value;

			if (IsDigit(dref string))
				value = dref string - '0';

			else
				break;

			result *= base;
			result += value;
			string++;
		}
	}
	else // Hex
	{
		for (;;)
		{
			u64 value;

			if (IsHex(dref string))
				value = HexTo_s32(dref string);

			else
				break;

			result *= base;
			result += value;
			string++;
		}
	}

	if (value)
		dref value = result;

	return (string - start);
}

InternLink sw
Format_Char(p<char> string_out, sw maxLength, p<FormatInfo> info, char arg)
{
	char 
	result[2] = "";
	result[0] = arg;

	return Format_Str(string_out, maxLength, info, result);
}

InternLink sw
Format_CharRepeated(p<char> string_out, sw maxLength, p<FormatInfo> info, char arg)
{
	sw result = 0;
	u32 remainder = (info) ? 
		info->Width > 0 ? 
			info->Width 
			:	1
		: 1;

	result = remainder;

	while (remainder > 0)
	{
		dref string_out = arg;

		string_out++;
		remainder--;
	}

	return result;
}

InternLink sw
Format_s64(p<char> string_out, sw maxLength, FormatInfo info, s64 value)
{
	char num[128];


}

InternLink sw
Format_u64(p<char> string_out, sw maxLength, FormatInfo info, u64 value)
{

}

InternLink sw
Format_f64(p<char> string_out, sw maxLength, FormatInfo info, u64 value)
{

}

InternLink sw
Format_Str(p<char> string_out, sw maxLength, p<FormatInfo> info, p<char const> string_in)
{
	sw
	result    = 0,
	length	  = 0,
	remaining = maxLength;

	p<char> begin = string_out;

	if (string_in == nullptr && maxLength >= 6)
	{
		result += Copy(string_out, "(null)", length);

		return result;
	}

	if (info == nullptr)
		return result;

	if (info->Precision)
		length = Length(string_in, info->Precision);

	else
		length = Length(string_in);

	if (info->Width == 0 && info->Flags & EFormat::Width)
	{
		return result;
	}

	if (info->Width == 0 || info->Flags & EFormat::Minus)
	{
		if (info->Precision > 0)
			length = info->Precision < length ? info->Precision : length;

		result		+= Copy(string_out, string_in, length);
		string_out	+= result;

		if (info->Width > result)
		{
			sw   padding = info->Width - length;
			char pad	 = (info->Flags & EFormat::Zero) ? '0' : ' ';

			while (padding > 0 && remaining > 0)
			{
				dref string_out = pad;

				string_out++;
				result++;
				padding--;
				remaining--;
			}

		}
	}
	else
	{
		if (info->Width > result)
		{
			sw   padding = info->Width - length;
			char pad	 = (info->Flags & EFormat::Zero) ? '0' : ' ';

			while (padding > 0 && remaining > 0)
			{
				dref string_out = pad;

				string_out++;
				result++;
				padding--;
				remaining--;
			}
		}

		result += Copy(string_out, string_in, length);
	}

	if (info->Flags & EFormat::Upper)
		ToUpper(begin);

	else if (info->Flags & EFormat::Lower)
		ToLower(begin);

	return result;
}

InternLink sw
Format_Str(p<char> string_out, sw maxLength, p<FormatInfo> info, p<char const> string_in, sw length)
{
	sw 
	result		= 0,
	remaining	= maxLength;

	p<char> begin = string_out;

	if (string_in == nullptr && maxLength >= 6)
	{
		result += CStr::Copy(string_out, "(null)", length);

		return result;
	}

	if (info == nullptr)
		return result;

	if (length != 0 && info->Precision < length)
		length = info->Precision;

	if (info->Width == 0 && info->Flags & EFormat::Width)
	{
		return result;
	}

	if (info->Width == 0 || info->Flags & EFormat::Minus)
	{
		if (info->Precision > 0)
			length = info->Precision < length ? info->Precision : length;

		result   += Copy(string_out, string_in, length);
		string_out += result;

		if (info->Width > result)
		{
			sw   padding = info->Width - length;
			char pad	 = (info->Flags & EFormat::Zero) ? '0' : ' ';

			while (padding > 0 && remaining > 0)
			{
				dref string_out = pad;

				string_out++;
				result++;
				padding--;
				remaining--;
			}
		}
	}
	else
	{
		if (info->Width > result)
		{
			sw   padding = info->Width - length;
			char pad	 = (info->Flags & EFormat::Zero) ? '0' : ' ';

			while (padding > 0 && remaining > 0)
			{
				dref string_out = pad;

				string_out++;
				result++;
				padding--;
				remaining--;
			}
		}

		result += Copy(string_out, string_in, length);
	}

	if (info->Flags & EFormat::Upper)
		ToUpper(begin);

	else if (info->Flags & EFormat::Lower)
		ToLower(begin);

	return result;
}
#pragma endregion Internal

} // CStr

