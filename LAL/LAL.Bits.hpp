#pragma once
#ifdef HPP_LAL_Bits
#pragma message("warning: HPP_LAL_Bit #pragma once failed.")
#endif
#ifndef HPP_LAL_Bits

#include "LAL/LAL.Casting.hpp"
#include "LAL.Metaprogramming.hpp"

LAL_NamespaceStart

template<TIntegral Representation>
constexpr Representation
bit(Representation pos)
{
	return 1 << pos;
}

#pragma region Trait
template<class Enum, class = void>	struct TAreBitFlags : TFalse {};
template<class Enum>				struct TAreBitFlags<Enum, HasMemberSymbol(Enum::TTrait_Enum_AreBitFlags)> : TIsEnum<Enum> {};

template<class Enum>	
constexpr bool AreBitFlags = TAreBitFlags<Enum>::Value && scast<uw>(Enum::TTrait_Enum_AreBitFlags) > uw(0) ? true : false;

template<class Enum>
concept TBitFlags = AreBitFlags<Enum>;
#pragma endregion Trait

/**
A wrapper object for bitfields that allows for typesafe bitmask operations.
*/
template<TBitFlags EType, class NativeRep>
struct Bitfield
{
public:
	using Type = Bitfield<EType, NativeRep>;

	using Enum           = EType;
	using Representation = NativeRep;

	static constexpr uDM NumBits = sizeof(Representation) * 8;

	Bitfield() : bitfield(0) {}

	Bitfield(Representation field) : bitfield(field)
	{}

	template<class... BitTypes>
	constexpr
	Bitfield(const BitTypes... bits) : bitfield(0)
	{
		bitfield = (Representation(bits) | ...);
	}

	template<class... BitType>
	ForceInline
	void Add(const BitType... bits)
	{
		bitfield |= (Representation(bits) | ...);
	}

	template<class... BitType>
	ForceInline
	bool CheckForEither(const BitType... bits) const
	{
		return (bitfield & (Representation(bits) | ...)) != 0;
	}

	template<class... BitType>
	ForceInline
	void Clear(const BitType... bits)
	{
		if (bitfield <= 0) 
			return;

		bitfield &= ~(Representation(_bits) | ...);
	}

	ForceInline
	bool HasFlag(const Enum bit) const
	{
		return (bitfield & Representation(bit)) == Representation(bit);
	}

	template<class... BitType>
	ForceInline
	bool HasExactly(const BitType... bits) const
	{
		return (bitfield & (Representation(bits) | ...)) == bitfield;
	}

	ForceInline
	bool HasAnyFlag() const 
	{
		return bitfield != 0 ? true : false; 
	}

	ForceInline
	bool IsZero() const 
	{
		return bitfield == 0 ? true : false; 
	}	

	ForceInline
	void Reset() 
	{
		bitfield = 0; 
	}

	template<class... BitType>
	ForceInline
	void Set(const BitType... bits)
	{
		bitfield = (Representation(bits) | ...);
	}

	template<class... BitType>
	ForceInline
	void Toggle(const BitType... bits)
	{
		bitfield ^= (Representation(bits) | ...);
	}

	operator Representation() const 
	{
		return bitfield; 
	}
	
	operator Bitset<NumBits>()
	{
		return bitfield;
	}

	Type& operator= (const Representation	mask ) { bitfield = mask; return *this; }
	Type& operator= (const Type				other) { bitfield = other.bitfield; return *this; }

	Type& operator&= (const Representation	mask ) { bitfield &= mask; return *this; }
	Type& operator&= (const Type			other) { bitfield &= other.bitfield; return *this; }

	Type& operator|= (const Representation	mask ) { bitfield |= mask; return *this; }
	Type& operator|= (const Type			other) { bitfield |= other.bitfield; return *this; }	

	Type& operator^= (const Representation	mask ) { bitfield ^= mask; return *this; }
	Type& operator^= (const Type			other) { bitfield ^= other.bitfield; return *this; }	

	Type& operator<<= (const Representation mask ) { bitfield <<= mask; return *this; }
	Type& operator>>= (const Type			other) { bitfield >>= other.bitfield; return *this; }	

	Type operator~ () const { return ~bitfield; }

	Representation operator& (const Representation	other) const { return bitfield & other; }
	Type		   operator& (const Type			other) const { return bitfield & other.bitfield; }

	Representation operator| (const Representation  other) const { return bitfield | other; }
	Type		   operator| (const Type			other) const { return bitfield | other.bitfield; }

	Representation operator^ (const Representation  other) const { return bitfield ^ other; }
	Type		   operator^ (const Type			other) const { return bitfield ^ other.bitfield; }

	Representation operator<< (const Representation other) const { return bitfield << other; }
	Type		   operator>> (const Type			other) const { return bitfield >> other.bitfield; }

	bool operator== (const Representation	other) const { return bitfield == other; }
	bool operator== (const Type				other) const { return bitfield == other.bitfield; }

	bool operator!= (const Representation	other) const { return bitfield != other; }
	bool operator!= (const Type				other) const { return bitfield != other.bitfield; }

#pragma region Vars
	Representation bitfield;
#pragma endregion Vars
};

#pragma region TypeSafeOps
#define TBitFlagsDef\
		template<TIntegral NativeT, TBitFlags Enum>

TBitFlagsDef NativeT operator &= (NativeT& ref, const Enum other) { return ref &= (NativeT) other; }
TBitFlagsDef NativeT operator |= (NativeT& ref, const Enum other) { return ref |= (NativeT) other; }

TBitFlagsDef constexpr NativeT operator & (NativeT a, Enum b) { return (NativeT)a & (NativeT)b; }
TBitFlagsDef constexpr NativeT operator | (NativeT a, Enum b) { return (NativeT)a | (NativeT)b; }

template<TBitFlags Enum> constexpr sw operator & (Enum a, Enum b) { return (sw)a & (sw)b; }
template<TBitFlags Enum> constexpr sw operator | (Enum a, Enum b) { return (sw)a | (sw)b; }

#undef TBitFlagsDef
#pragma endregion TypeSafeOps

LAL_NamespaceEnd
#define HPP_LAL_Bits
#endif

