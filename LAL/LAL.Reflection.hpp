#pragma once
#include "LAL.Types.hpp"

#ifndef HPP_LAL_Reflection
LAL_NamespaceStart

#pragma region Macros
#define HasMemberSymbol(Member_Symbol_)\
	decltype(static_cast<void>(Member_Symbol_))

#define HasMemberAddress(Member_Address_)\
	Member_Address_ != nullptr ? true : false
#pragma endregion macros

#pragma region Removes
template<class TType>			struct RemoveConst						{ typedef TType Type; };
template<class TType>			struct RemoveConst<const TType>			{ typedef TType Type; };
template<class TType>			struct RemoveConst<const TType[]>		{ typedef TType Type[]; };
template<class TType, uw Size>	struct RemoveConst<const TType[Size]>	{ typedef TType Type[Size]; };

template<class TType>
using TRemoveConst = typename RemoveConst<TType>::Type;

template<class TType>			struct RemoveVolatile						{ typedef TType Type; };
template<class TType>			struct RemoveVolatile<volatile TType>		{ typedef TType Type; };
template<class TType>			struct RemoveVolatile<volatile TType[]>		{ typedef TType Type[]; };
template<class TType, uw Size>	struct RemoveVolatile<volatile TType[Size]>	{ typedef TType Type[Size]; };

template<class TType>
using TRemoveVolatile = typename RemoveVolatile<TType>::Type;

template<class TType> 
struct RemoveCV
{
	typedef 
	typename RemoveVolatile<
	typename RemoveConst<TType>::Type
	>::Type 

	Type;
};

template<class TType>
using TRemoveCV = typename RemoveCV<TType>::Type;
#pragma endregion Removes

template<class TValType, TValType tValue>
struct TranslationConstant
{
	using Type		= TranslationConstant<TValType, tValue>;
	using ValueType = TValType;

	static constexpr ValueType Value = tValue;

	constexpr operator
		ValueType() const noexcept
	{
		return Value;
	}

	constexpr ValueType 
		operator()() const noexcept
	{
		return Value;
	}
};

#pragma region TBool
template<bool Value>
struct TBool : public TranslationConstant<bool, Value> {};

//using TTrue  = TranslationConstant<bool, true>;
//using TFalse = TranslationConstant<bool, false>;
typedef TranslationConstant<bool, true>	TTrue;
typedef TranslationConstant<bool, true>	TFalse;
#pragma region TBool

#pragma region IsSame
template<class Type, class Other>
struct TIsSame : public TFalse {};

template<class Type>
struct TIsSame<Type, Type> : public TTrue {};

template<class Type, class Other> 
constexpr bool 
IsSame()
{
	return TIsSame<Type, Other>::Value;
}
#pragma endregion IsSame

#pragma region Void
template<class Type> struct TIsVoid : public TFalse {};

template<> struct TIsVoid<void>					: public TTrue {};
template<> struct TIsVoid<void const>			: public TTrue {};
template<> struct TIsVoid<void		 volatile>	: public TTrue {};
template<> struct TIsVoid<void const volatile>	: public TTrue {};

template<class Type>
constexpr bool
IsVoid()
{
	return TIsVoid<Type>::Value;
}

template<class ...Args>				struct THasVoid_Arg;
template<>							struct THasVoid_Arg<> : public TFalse {};
template<class ArgA, class ...Args> struct THasVoid_Arg<ArgA, Args...>
{
	static const bool 
	Value = (TIsVoid<ArgA>::Value || THasVoid_Arg<Args...>::Value);
};
#pragma endregion Void

#pragma region IntegralType
template <class Type> struct TIsIntergralResolver			: public TFalse {};

template <> struct TIsIntergralResolver<unsigned char>      : public TTrue {};
template <> struct TIsIntergralResolver<unsigned short>     : public TTrue {};
template <> struct TIsIntergralResolver<unsigned int>       : public TTrue {};
template <> struct TIsIntergralResolver<unsigned long>      : public TTrue {};
template <> struct TIsIntergralResolver<unsigned long long> : public TTrue {};

template <> struct TIsIntergralResolver<signed char>        : public TTrue {};
template <> struct TIsIntergralResolver<signed short>       : public TTrue {};
template <> struct TIsIntergralResolver<signed int>         : public TTrue {};
template <> struct TIsIntergralResolver<signed long>        : public TTrue {};
template <> struct TIsIntergralResolver<signed long long>   : public TTrue {};

template <> struct TIsIntergralResolver<bool>               : public TTrue {};
template <> struct TIsIntergralResolver<char>               : public TTrue {};
template <> struct TIsIntergralResolver<char16_t>			: public TTrue {};
template <> struct TIsIntergralResolver<char32_t>			: public TTrue {};
#ifndef char_t_NonNative // If wchar_t is a native type instead of simply a define to an existing type which is already handled above...
template <> struct TIsIntergralResolver<wchar_t>			: public TTrue {};
#endif
#if M_Int128Supported && (defined(Compiler_GCC) || defined(Compiler_Clang))
template <> struct TIsIntergralResolver<__int128_t>			: public TTrue {};
template <> struct TIsIntergralResolver<__uint128_t>		: public TTrue {};
#endif

template <class Type>
struct TIsIntegral : public TIsIntergralResolver<typename RemoveCV<Type>::Type> {};

template<class TType>
constexpr bool
IsIntegral()
{
	return TIsIntegral<TType>::Value;
}

template<class Type>
concept TIntegral = TIsIntegral<Type>::Value;

#define LAL_DeclareIntegralTrait(Type_)										\
template <> struct IsIntegral<					Type_> : public TTrue {};   \
template <> struct IsIntegral<const				Type_> : public TTrue {};   \
template <> struct IsIntegral<		volatile	Type_> : public TTrue {};   \
template <> struct IsIntegral<const volatile	Type_> : public TTrue {};
#pragma endregion IntegralType

#pragma region UnsignedType
template <class Type> struct TIsUnsignedResolver			: public TFalse {};

template <> struct TIsUnsignedResolver<unsigned char>		: public TTrue {};
template <> struct TIsUnsignedResolver<unsigned short>		: public TTrue {};
template <> struct TIsUnsignedResolver<unsigned int>		: public TTrue {};
template <> struct TIsUnsignedResolver<unsigned long>		: public TTrue {};
template <> struct TIsUnsignedResolver<unsigned long long>	: public TTrue {};
#if M_Int128Supported && (defined(Compiler_GCC) || defined(Compiler_Clang))
template <> struct TIsUnsignedResolver<__uint128_t>			: public TTrue {};
#endif

template <class Type>
struct TIsUnsigned : public TIsUnsignedResolver<typename RemoveCV<Type>::Type> {};

template<class TType>
constexpr bool
IsUnsigned()
{
	return TIsUnsigned<TType>::Value;
}

template<class Type>
concept TUnsigned = TIsUnsigned<Type>::Value;

#define LAL_DeclareUnsignedTrait(Type_)										\
template <> struct IsUnsigned<					Type_> : public TTrue {};   \
template <> struct IsUnsigned<const				Type_> : public TTrue {};   \
template <> struct IsUnsigned<		volatile	Type_> : public TTrue {};   \
template <> struct IsUnsigned<const volatile	Type_> : public TTrue {};
#pragma endregion UnsignedType

#pragma region Pointers
template<class TType> struct TIsNullPtr : public
	TIsSame< TRemoveCV<TType>, nullptrType> 
{};
#pragma endregion Pointers

LAL_NamespaceEnd
#define HPP_LAL_Reflection
#endif

