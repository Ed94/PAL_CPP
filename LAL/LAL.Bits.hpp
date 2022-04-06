#pragma once
#ifdef HPP_LAL_Bits
#pragma message("warning: HPP_LAL_Bit #pragma once failed.")
#endif
#ifndef HPP_LAL_Bits

#include "LAL.Metaprogramming.hpp"

LAL_NamespaceStart

template<TIntegral Representation>
constexpr Representation
bit(Representation pos)
{
	return 1 << pos;
}

#pragma region BitmaskableTraits
template<typename Enum, typename = void>	struct TIsBitmaskable : TFalse {};
template<typename Enum>						struct TIsBitmaskable<Enum, HasMemberSymbol(Enum::SpecifyBitmaskable)> : TIsEnum<Enum> {};

template<typename Enum>	
constexpr bool IsBitmaskable = TIsBitmaskable<Enum>::Value;

template <typename Enum>
constexpr EnableIf<IsBitmaskable<Enum>, bool> 
Bitmaskable() noexcept {
	return scast<uw>(Enum::SpecifyBitmaskable) > uw(0) ? true : false;
}

template <typename Enum> 
constexpr EnableIf<! IsBitmaskable<Enum>, bool> 
Bitmaskable() noexcept {
	return false;
}
#pragma endregion BitmaskableTraits

/**
A wrapper object for bitfields that allows for typesafe bitmask operations.
*/
template<class EType, class NativeRep>
struct Bitfield
{
private:
	static_assert(Bitmaskable<EType>(), "EnumType must be of Bitmaskable type.");

	using Type = Bitfield<EType, NativeRep>;

public:

	using Enum           = EType;
	using Representation = NativeRep;

	static constexpr uDM NumBits = sizeof(Representation) * 8;

	Bitfield() : bitfield(0) {}

	Bitfield(Representation field) : bitfield(field)
	{}

	template<typename... BitTypes>
	constexpr
	Bitfield(const BitTypes... bits) : bitfield(0)
	{
		bitfield = (Representation(bits) | ...);
	}

	template<typename... BitType>
	ForceInline
	void Add(const BitType... bits)
	{
		bitfield |= (Representation(bits) | ...);
	}

	template<typename... BitType>
	ForceInline
	bool CheckForEither(const BitType... bits) const
	{
		return (bitfield & (Representation(bits) | ...)) != 0;
	}

	template<typename... BitType>
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

	template<typename... BitType>
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

	template<typename... BitType>
	ForceInline
	void Set(const BitType... bits)
	{
		bitfield = (Representation(bits) | ...);
	}

	template<typename... BitType>
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

LAL_NamespaceEnd
#define HPP_LAL_Bits
#endif

