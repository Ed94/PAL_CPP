#pragma once

#include "LAL.Asserts.hpp"
#include "LAL.Casting.hpp"
#include "LAL.Declarations.hpp"
#include "LAL.Math.hpp"
#include "LAL.Types.hpp"
#include "OSAL.Platform.hpp"

#ifndef HPP_LAL_Memory
LAL_NamespaceStart
namespace Memory
{
    ForceInline p<void>
	Align_Forward(p<void> address, sw alignment)
    {
        assert(IsPowerOfTwo(alignment));

        uiPtr intAddr = rcast<uiPtr>(address);

        return rcast< p<void>>((intAddr + (alignment -1)) & ~(alignment -1));
    }

    ForceInline s64
	Align_Forward(s64 value, sw alignment)
    {
        return value + (alignment - value % alignment) % alignment;
    }

    ForceInline u64
	Align_Forward(u64 value, uw alignment)
    {
        return value + (alignment - value % alignment) % alignment;
    }

    template<class Type>
    ForceInline s32
	Compare(p<void const> segA, p<void const> segB, sw size)
    {
        p<u8 const> segA_bytes = rcast< p<u8 const>>(segA);
        p<u8 const> segB_bytes = rcast< p<u8 const>>(segB);

        if (segA == nullptr || segB == nullptr)
            return 0;

        while (size--)
        {
            sw diff;
            if ((diff = (dref segA_bytes++ - dref segB_bytes++)) != 0)
                return rcast<s32>(diff);
        }

        return 0;
    }

    template<class Type> 
    ForceInline p<Type>
	Format_ByFill(p<Type> address, Type fillValue, uw count)
    {
        if (address == nullptr) 
            return nullptr;

        p<u8> bytes = rcast< p<u8>>(address);

        sw k;

        u32 c32 = u32(-1) / 255 * fillValue;

        if (count == 0) 
            return address;

        bytes[0] = bytes[count - 1] = fillValue;

        if (count < 3) 
            return address;

        bytes[1] = bytes[count - 2] = fillValue;
        bytes[2] = bytes[count - 3] = fillValue;

        if (count < 7) 
            return address;

        bytes[3] = bytes[count - 4] = fillValue;

        if (count < 9) 
            return address;

        k      = -siPtr(bytes) & 3;
        bytes += k;
        count -= k;
        count &= -4;

        dref p<u32>(bytes + 0)            = c32;
        dref p<u32>(bytes + count - 4)    = c32;

        if (count < 9) 
            return address;

        dref p<u32>(bytes + 4)            = c32;
        dref p<u32>(bytes + 8)            = c32;
        dref p<u32>(bytes + count - 12)   = c32;
        dref p<u32>(bytes + count - 8)    = c32;

        if (count < 25) 
            return address;

        dref p<u32>(bytes + 12)           = c32;
        dref p<u32>(bytes + 16)           = c32;
        dref p<u32>(bytes + 20)           = c32;
        dref p<u32>(bytes + 24)           = c32;
        dref p<u32>(bytes + count - 28)   = c32;
        dref p<u32>(bytes + count - 24)   = c32;
        dref p<u32>(bytes + count - 20)   = c32;
        dref p<u32>(bytes + count - 16)   = c32;

        k = 24 + uiPtr(bytes) & 4;

        bytes += k;
        count -= k;
        {
            u64 c64 = u64(c32) << 32 | c32;

            while (count > 31) 
            {
                dref p<u64>(bytes + 0)    = c64;
                dref p<u64>(bytes + 8)    = c64;
                dref p<u64>(bytes + 16)   = c64;
                dref p<u64>(bytes + 24)   = c64;

                count -= 32;
                bytes += 32;
            }
        }

        return address;
    }

    template<class Type>
    ForceInline p<Type>
	Format_WithData(p<Type> address, p<Type const> source, uw count)
    {
        if constexpr (! IsSame<Type, void>())
			count *= sizeof(Type);

		return memcpy(address, source, count);
    }

    template<class Type>
    ForceInline p<Type>
	Format_WithZero(p<Type> address, uw count)
    {
        return Format_ByFill<Type>(address, 0, count);
    }

	template<class Type> 
	ForceInline p<Type>
	Format_ZeroItem(Type& item)
	{
		return rcast<p<Type>>(Format_ByFill<u8>(rcast<p<u8>>(ptrof item), 0, sizeof(Type)));
	}

    template<class Type>
    ForceInline p<Type>
	Move(p<Type> address, p<Type const> source, uw count)
    {
        if (address == nullptr)
            return nullptr;

        if constexpr (! IsSame<Type, void>())
			count *= sizeof(Type);

        p<u8>         addrBytes = rcast< p<u8>>         (address);
        p<u8 const>   srcBytes  = rcast< p<u8 const>>   (source);

        if (addrBytes == srcBytes) 
            return address;

        // NOTE: Non-overlapping
        if (srcBytes + count <= addrBytes || addrBytes + count <= srcBytes) 
            return Format_WithData(address, source, count);

        if (addrBytes < srcBytes) 
        {
            if (uiPtr(srcBytes) % sizeof(sw) == uiPtr(addrBytes) % sizeof(sw)) 
            {
                while (uiPtr(addrBytes) % sizeof(sw)) 
                {
                    if (! count--)
                        return address;

                    dref addrBytes++ = dref srcBytes++;
                }

                while (count >= sizeof(sw)) 
                {
                    dref p<sw>(addrBytes) = dref p<sw>(srcBytes);

                    count     -= sizeof(sw);
                    addrBytes += sizeof(sw);
                    srcBytes  += sizeof(sw);
                }
            }

            for (; count; count--) 
                *addrBytes++ = *srcBytes++;
        } 
        else 
        {
            if ((uiPtr(srcBytes) % sizeof(sw)) == (uiPtr(addrBytes) % sizeof(sw))) 
            {
                while (uiPtr(addrBytes + count) % sizeof(sw)) 
                {
                    if (! count--) 
                        return address;

                    addrBytes[count] = srcBytes[count];
                }

                while (count >= sizeof(sw)) 
                {
                    count -= sizeof(sw);

                    dref p<sw>(srcBytes + count) = dref p<sw>(srcBytes + count);
                }
            }

            while (count) 
                count--, addrBytes[count] = srcBytes[count];
        }

        return address;
    }

    ForceInline p<void>
	Ptr_AddBytes(p<void> address, sw bytes)
    {
        return rcast< p<void>>(rcast< p<u8>>(address) + bytes);
    }

    ForceInline p<void const>
	Ptr_AddBytes(p<void const> address, sw bytes)
    {
        return rcast< p<void const>>(rcast< p<u8 const>>(address) + bytes);
    }

    ForceInline p<void>
	Ptr_SubBytes(p<void> address, sw bytes)
    {
        return rcast< p<void>>(rcast< p<u8>>(address) - bytes);
    }

    ForceInline p<void const>
	Ptr_SubBytes(p<void const> address, sw bytes)
    {
        return rcast< p<void const>>(rcast< p<u8 const>>(address) - bytes);
    }

    ForceInline sw
	Ptr_ByteDiff(p<void const> ptrA, p<void const> ptrB)
    {
        return sw(p<u8 const>(ptrB) - p<u8 const>(ptrA));
    }
}
LAL_NamespaceEnd
#define HPP_LAL_Memory
#endif

