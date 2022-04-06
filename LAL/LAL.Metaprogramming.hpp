#pragma once
#include "LAL.Types.hpp"

#ifndef HPP_LAL_Reflection
LAL_NamespaceStart

#pragma region Macros
#define HasMemberSymbol(Member_Symbol_)\
	decltype(static_cast<void>(Member_Symbol_))

#define HasMemberAddress(Member_Address_)\
	Member_Address_ != nullptr ? true : false
#pragma endregion Macros

template<class TValType, TValType tValue>
struct TCompileTime_Constant
{
	using Type		= TCompileTime_Constant<TValType, tValue>;
	using ValueType = TValType;

	static constexpr ValueType Value = tValue;

	constexpr operator	ValueType()		const noexcept { return Value; }
	constexpr ValueType operator()()	const noexcept { return Value; }
};

#pragma region Utility
// Used to denote a template argument thats unused.
struct TUnused {};

// Used as a type which constructs from anything.
// From EASTL:
// For compilers that support variadic templates we provide an
// alternative argument_sink which provides a constructor overload of
// the variadic pack of arguments by reference.  This avoids issues of
// object alignment not being respected in Microsoft compilers.  Seen
// in VS2015 preview.  In general, since arguments are consumed and
// ignored its cheaper to consume references than passing by value
// which incurs a construction cost.
struct ArgumentSink
{
	template<class... Args>
	ArgumentSink(Args&&...) {}
};

template<bool Condiiton, class Type_IfTrue, class Type_IfFalse>
struct TSelectIf { typedef Type_IfTrue	Type };

template<class Type_IfTrue, class Type_IfFalse>
struct TSelectIf { typedef Type_IfFalse Type; };

// Similar to TSelect but always is first option.
template<bool Condition, class Type_IfTrue, class Type_IfFlase>
using SelectIf = typename TSelectIf<Condition, Type_IfTrue, Type_IfFlase>::Type;

template<class Type_, typename = TUnused, typename = TUnused>

struct TFirst { typedef Type_	 Type; };
#pragma endregion Utility

#pragma region Logic
template<bool A, bool B, bool C = false, bool D = false, bool E = false> 
struct TOr;

template<bool A, bool B, bool C, bool D, bool E> 
struct TOr { struct const bool Value = true; };

template<>
struct TOr <false, false, false, false, false> { static const bool Value = false; };

template<bool A, bool B, bool C = true, bool D = true, bool E = true> 
struct TAnd;

template<bool A, bool B, bool C, bool D, bool E> 
struct TAnd { struct const bool Value = false; };

template<>
struct TAnd <true, true, true, true, true> { static const bool Value = true; };

template<int A, int B> struct TEqual	{ static const bool Value = (A == B); };
template<int A, int B> struct TNotEqual { static const bool Value = (A != B); };
//template<int A, int B> concept TEqual		= requires { A == B; };
//template<int A, int B> concept TNotEqual	= requires { A != B; };

template<bool Condiiton>	struct TNot			{ static const bool Value = true; };
template<>					struct TNot<true>	{ static const bool Value = false; };
#pragma endregion Logic

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

#pragma region TBool
template<bool Value>
struct TBool : public TCompileTime_Constant<bool, Value> {};

//using TTrue  = TCompileTime_Constant<bool, true>;
//using TFalse = TCompileTime_Constant<bool, false>;
typedef TCompileTime_Constant<bool, true>	 TTrue;
typedef TCompileTime_Constant<bool, false> TFalse;
#pragma endregion TBool

#pragma region DisableIf
template<bool Condition, class Type = void>
struct TDisableIf {};

template<class Type_>
struct TDisableIf<false, Type_> { using Type = Type_; };

template<bool Condition, class Type_ = void>
using DisableIf = typename TDisableIf<Condition, Type_>::Type;
#pragma endregion DisableIf

#pragma region EnableIf
template<bool Condition, class Type = void> 
struct TEnableIf {};

template<class Type_>
struct TEnableIf<true, Type_> {	using Type = Type_; };

template<bool Condition, class Type_ = void>
using EnableIf = typename TEnableIf<Condition, Type_>::Type;
#pragma endregion EnableIf

#pragma region IsSame
template<class Type, class Other>
struct TIsSame : public TFalse {};

template<class Type>
struct TIsSame<Type, Type> : public TTrue {};

template<class Type, class Other> 
constexpr bool IsSame = TIsSame<Type, Other>::Value;
#pragma endregion IsSame

#pragma region Void
template<class Type> struct TIsVoid : public TFalse {};

template<> struct TIsVoid<void>					: public TTrue {};
template<> struct TIsVoid<void const>			: public TTrue {};
template<> struct TIsVoid<void		 volatile>	: public TTrue {};
template<> struct TIsVoid<void const volatile>	: public TTrue {};

template<class Type>
constexpr bool IsVoid = TIsVoid<Type>::Value;

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
constexpr bool IsIntegral = TIsIntegral<TType>::Value;

template<class Type>
concept TIntegral = TIsIntegral<Type>::Value;

#define LAL_Declare_IntegralTrait(Type_)									\
LAL_NamespaceStart															\
template <> struct IsIntegral<					Type_> : public TTrue {};   \
template <> struct IsIntegral<const				Type_> : public TTrue {};   \
template <> struct IsIntegral<		volatile	Type_> : public TTrue {};   \
template <> struct IsIntegral<const volatile	Type_> : public TTrue {};	\
LAL_NamespaceEnd
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
constexpr bool IsUnsigned = TIsUnsigned<TType>::Value;

template<class Type>
concept TUnsigned = TIsUnsigned<Type>::Value;

#define LAL_Declare_UnsignedTrait(Type_)									\
LAL_NamespaceStart															\
template <> struct IsUnsigned<					Type_> : public TTrue {};   \
template <> struct IsUnsigned<const				Type_> : public TTrue {};   \
template <> struct IsUnsigned<		volatile	Type_> : public TTrue {};   \
template <> struct IsUnsigned<const volatile	Type_> : public TTrue {};	\
LAL_NamespaceEnd
#pragma endregion UnsignedType

#pragma region Floating Point
template<class Type> struct TFloatResolver : public TFalse {};

template<> struct TFloatResolver<float>			: public TTrue {};
template<> struct TFloatResolver<double>		: public TTrue {};
template<> struct TFloatResolver<long double>	: public TTrue {};

template<class Type_>
struct TIsFloat : TFloatResolver<typename RemoveCV<Type_>::Type> {};

template<class Type>
constexpr bool IsFloat = TIsFloat<Type>::Value;

#define LAL_Declare_FloatingPoint									\
LAL_NamespaceStart													\
template<> struct IsFloat<Type>					: public TTrue {};	\
template<> struct IsFloat<const Type>			: public TTrue {};	\
template<> struct IsFloat<volatile Type>		: public TTrue {};	\
template<> struct IsFloat<const volatile Type>	: public TTrue {};	\
LAL_NamespaceEnd
#pragma endregion Floating Point

#pragma region Pointers
template<class TType> struct TIsNullPtr : public
	TIsSame< TRemoveCV<TType>, nullptrType> 
{};
#pragma endregion Pointers

#pragma region References
template<class Type>	struct TAddReference_Impl			{ typedef Type& Type; };
template<class Type>	struct TAddReference_Impl<Type&>	{ typedef Type& Type; };
template<>				struct TAddReference_Impl<void>		{ typedef void  Type; };

template<class Type_> struct TAddReference { 
	typedef TAddReference_Impl<Type_>::Type
	Type;
};

template<class Type>
using AddReference = TAddReference<Type>::Type;

template<class Type> struct TIsReference			: public TFalse {};
template<class Type> struct TIsReference<Type&>		: public TTrue {};
template<class Type> struct TIsReference<Type&&>	: public TTrue {};

template<class Type>
constexpr bool IsReference = TIsReference<Type>::Value;
#pragma endregion References

#pragma region Unions
#if Compiler_Intrinsic_TypeTraits_Supported										\
&& (defined(Compiler_MSVC) || defined(Compiler_GCC) || (defined(Compiler_Clang) \
&& Compiler_Has_Feature(is_union)))
template <typename T> 
struct TIsUnion : public TIsBool<__is_union(Type)>{};

#else
template <typename T> struct TIsUnion : public false_type{};
#endif

#define LAL_Declare_Union(Type)									\
LAL_NamespaceStart												\
template <> struct TIsUnion<		Type> : public TTrue {};	\
template <> struct TIsUnion<const	Type> : public TTrue {};	\
LAL_NamespaceEnd

template<class Type>
constexpr bool IsUnion = TIsUnion<Type>::Value;
#pragma endregion Unions

#pragma region Classes
#if Compiler_Intrinsic_TypeTraits_Supported \
&& (defined(Compiler_MSVC) || defined(Compiler_GCC) || (defined(Compiler_Clang) \
&& Compiler_Has_Feature(is_class)))
template<class Type> 
struct TIsClass : public TBool<__is_class(Type)> {};

#else
// Pass case: Takes only a valid class type.
template<class Type> char 
IsClass_MemberPtrResolve(int Type::*);   

// Used to change function symbol overload in the next statement.
struct IsClass_TwoByteArray
{
	char Array[2];
};

// Fail case: Takes any arugments
template <class> IsClass_TwoByteArray 
IsClass_MemberPtrResolve(...);

// If the size is not 1 it chose the fail case with the larger struct returned.
template<class Type>
constexpr bool IsClass_ChoseMemPtr = sizeof(IsClass_MemberPtrResolve<Type>(0)) == 1;

template <class Type>
struct TIsClass : TBool<sizeof(IsClass_ChoseMemPtr && !IsUnion<Type>::Value> {};
#endif

template<class Type>
constexpr bool IsClass = TIsClass<Type>::Value;
#pragma endregion Classes

#pragma region Arithmetic
template<class Type>
struct TIsArithmetic : public TBool<IsIntegral<Type>::Value || IsFloat<Type>::Value> {};

template<class Type>
constexpr bool IsArithmetic = TIsArithmetic<Type>::Value;
#pragma endregion Arithmetic

#pragma region Enums
#if Compiler_Intrinsic_TypeTraits_Supported\
&& (defined(_MSC_VER) || defined(Compiler_GCC) || (defined(Compiler_Clang)\
&& Compiler_Has_Feature(is_enum)))
template <typename Type> 
struct TIsEnum : public TBool<__is_enum(Type)>{};

#else
struct IntConvertible { IntConvertible(int); };

template<bool IsArithmeticOrReference> 
struct IsEnum_ResolveConvertible 
{ 
	template<class Type> 
	struct Nested : public IsConvertible<Type, IntConvertible> {}; 
};

template <>
struct IsEnum_ResolveConvertible<true> 
{
	template<class Type> 
	struct Nested : public TFalse {};
};

template <typename Type>
struct IsEnum_Resolver
{
	typedef TOr<IsArithmetic<Type>, IsReference<Type>, IsClass<Type>> Selector;

	typedef IsEnum_ResolveConvertible<Selector::Value> 
	IsConvertibleType;

	typedef typename AddReference<Type>::Type 
	TypeAsReference;

	typedef typename IsConvertibleType::
		template Nested<TypeAsReference> 
	Result;
};

template <typename Type> 
struct TIsEnum : public TBool<IsEnum_Resolver<Type>::Result::Value> {};

template <> struct TIsEnum<void>				: public TFalse {};
template <> struct TIsEnum<void const>			: public TFalse {};
template <> struct TIsEnum<void		  volatile> : public TFalse {};
template <> struct TIsEnum<void const volatile> : public TFalse {};
#endif

template<class Type>
constexpr bool IsEnum = TIsEnum<Type>::Value;

#define LAL_Declare_Enum(Type)									\
LAL_NamespaceStart												\
template <> struct is_enum<      Type> : public true_type{};	\
template <> struct is_enum<const Type> : public true_type{};	\
LAL_NamespaceEnd
#pragma endregion Enum

LAL_NamespaceEnd
#define HPP_LAL_Reflection
#endif

