#pragma once
#include "LAL.Types.hpp"

LAL_NamespaceStart

#pragma region Macros
#define HasMemberSymbol(Member_Symbol_)\
		decltype(static_cast<void>(Member_Symbol_))

#define HasMemberAddress(Member_Address_)\
		Member_Address_ != nullptr ? true : false
#pragma endregion Macros

#pragma region Utility
// is_constant_evaluated
[[nodiscard]] constexpr bool
IsCompileTime()
{
#ifdef Compiler_MSVC
	return __builtin_is_constant_evaluated();

#else
	static_assert(false, "Do not have implementation for platform.");
#endif
}

// integral_constant
template<class TValType, TValType tValue>
struct TConstant
{
	using Type		= TConstant<TValType, tValue>;
	using ValueType = TValType;

	static constexpr ValueType Value = tValue;

	constexpr operator	ValueType()		const noexcept { return Value; }
	constexpr ValueType operator()()	const noexcept { return Value; }
};

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

template<class... TArgs> struct ArgTypes {};

template<bool Condiiton, class TTypeIfTrue, class TTypeIfFalse>
struct TSelectIf { typedef TTypeIfTrue	Type };

template<class TTypeIfTrue, class TTypeIfFalse>
struct TSelectIf { typedef TTypeIfFalse Type; };

// Similar to TSelect but always is first option.
template<bool Condition, class TTypeIfTrue, class TTypeIfFlase>
using SelectIf = typename TSelectIf<Condition, TTypeIfTrue, TTypeIfFlase>::Type;

template<class TType, typename = TUnused, typename = TUnused>
struct TFirst { typedef TType	 Type; };

template<class TType>
struct IdentityType { using Type = TType; };

template<class TType>
using TIdentity = IdentityType<TType>::Type;

template<class... TArgs> void Swallow(TArgs&&...) {};
#pragma endregion Utility

#pragma region Removes
template<class TType> struct RemoveRef          { typedef TType Type: };
template<class TType> struct RemoveRef<TType&>  { typedef TType Type; };
template<class TType> struct RemoveRef<TType&&> { typedef TType Type; };

template<class TType>
using TRemoveRef = typename RemoveRef<TType>::Type;

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

template<bool Condiiton>	struct TNot			{ static const bool Value = true; };
template<>					struct TNot<true>	{ static const bool Value = false; };
#pragma endregion Logic

#pragma region TBool
template<bool Value>
struct TBool : TConstant<bool, Value> {};

typedef TConstant<bool, true>  TTrue;
typedef TConstant<bool, false> TFalse;

template<class Type> constexpr bool AlwaysFalse = false;
#pragma endregion TBool

#pragma region DisableIf
template<bool Condition, class Type = void>
struct TDisableIf {};

template<class TType>
struct TDisableIf<false, TType> { using Type = TType; };

template<bool Condition, class TType = void>
using DisableIf = typename TDisableIf<Condition, TType>::Type;
#pragma endregion DisableIf

#pragma region EnableIf
template<bool Condition, class Type = void> 
struct TEnableIf {};

template<class TType>
struct TEnableIf<true, TType> {	using Type = TType; };

template<bool Condition, class TType = void>
using EnableIf = typename TEnableIf<Condition, TType>::Type;
#pragma endregion EnableIf

#pragma region IsSame
template<class Type, class Other>
struct TIsSame : TFalse {};

template<class Type>
struct TIsSame<Type, Type> : TTrue {};

template<class Type, class Other> 
constexpr bool IsSame = TIsSame<Type, Other>::Value;

template<class Type, class Other>
concept TSame = TIsSame<Type, Other>::Value;
#pragma endregion IsSame

#pragma region Void
template<class Type> struct TIsVoid : TFalse {};

template<> struct TIsVoid<void>					: TTrue {};
template<> struct TIsVoid<void const>			: TTrue {};
template<> struct TIsVoid<void		 volatile>	: TTrue {};
template<> struct TIsVoid<void const volatile>	: TTrue {};

template<class Type>
constexpr bool IsVoid = TIsVoid<Type>::Value;

template<class Type>
concept TVoid = IsVoid<Type>;

template<class ...Args>				struct THasVoid_Arg;
template<>							struct THasVoid_Arg<> : TFalse {};
template<class ArgA, class ...Args> struct THasVoid_Arg<ArgA, Args...>
{
	static const bool 
	Value = (TIsVoid<ArgA>::Value || THasVoid_Arg<Args...>::Value);
};
#pragma endregion Void

#pragma region IntegralType
template <class Type> struct TIsIntergralResolver			: TFalse {};

template <> struct TIsIntergralResolver<unsigned char>      : TTrue {};
template <> struct TIsIntergralResolver<unsigned short>     : TTrue {};
template <> struct TIsIntergralResolver<unsigned int>       : TTrue {};
template <> struct TIsIntergralResolver<unsigned long>      : TTrue {};
template <> struct TIsIntergralResolver<unsigned long long> : TTrue {};

template <> struct TIsIntergralResolver<signed char>        : TTrue {};
template <> struct TIsIntergralResolver<signed short>       : TTrue {};
template <> struct TIsIntergralResolver<signed int>         : TTrue {};
template <> struct TIsIntergralResolver<signed long>        : TTrue {};
template <> struct TIsIntergralResolver<signed long long>   : TTrue {};

template <> struct TIsIntergralResolver<bool>               : TTrue {};
template <> struct TIsIntergralResolver<char>               : TTrue {};
template <> struct TIsIntergralResolver<char16_t>			: TTrue {};
template <> struct TIsIntergralResolver<char32_t>			: TTrue {};
#ifndef char_t_NonNative // If wchar_t is a native type instead of simply a define to an existing type which is already handled above...
template <> struct TIsIntergralResolver<wchar_t>			: TTrue {};
#endif
#if M_Int128Supported && (defined(Compiler_GCC) || defined(Compiler_Clang))
template <> struct TIsIntergralResolver<__int128_t>			: TTrue {};
template <> struct TIsIntergralResolver<__uint128_t>		: TTrue {};
#endif

template <class Type>
struct TIsIntegral : public TIsIntergralResolver<typename RemoveCV<Type>::Type> {};

template<class TType>
constexpr bool IsIntegral = TIsIntegral<TType>::Value;

template<class Type>
concept TIntegral = TIsIntegral<Type>::Value;

#define LAL_Declare_IntegralTrait(TType)							\
		LAL_NamespaceStart											\
		template <> struct TIsIntegralResolver<TType> : TTrue {};   \
		LAL_NamespaceEnd
#pragma endregion IntegralType

#pragma region UnsignedType
template <class Type> struct TIsUnsignedResolver			: TFalse {};

template <> struct TIsUnsignedResolver<unsigned char>		: TTrue {};
template <> struct TIsUnsignedResolver<unsigned short>		: TTrue {};
template <> struct TIsUnsignedResolver<unsigned int>		: TTrue {};
template <> struct TIsUnsignedResolver<unsigned long>		: TTrue {};
template <> struct TIsUnsignedResolver<unsigned long long>	: TTrue {};
#if M_Int128Supported && (defined(Compiler_GCC) || defined(Compiler_Clang))
template <> struct TIsUnsignedResolver<__uint128_t>			: TTrue {};
#endif

template <class Type>
struct TIsUnsigned : TIsUnsignedResolver<typename RemoveCV<Type>::Type> {};

template<class TType>
constexpr bool IsUnsigned = TIsUnsigned<TType>::Value;

template<class Type>
concept TUnsigned = TIsUnsigned<Type>::Value;

#define LAL_Declare_UnsignedTrait(TType)							\
		LAL_NamespaceStart											\
		template <> struct IsUnsignedResolver<TType> : TTrue {};	\
		LAL_NamespaceEnd
#pragma endregion UnsignedType

#pragma region Chars
template <class Type> struct TIsCharResolver : TFalse {};

template <> struct TIsCharResolver<char>	: TTrue {};
template <> struct TIsCharResolver<wchar_t> : TTrue {};
template <> struct TIsCharResolver<ascii>	: TTrue {};
template <> struct TIsCharResolver<wchar>	: TTrue {};
template <> struct TIsCharResolver<char8>	: TTrue {};
template <> struct TIsCharResolver<char16>	: TTrue {};
template <> struct TIsCharResolver<char32>	: TTrue {};

template<class TType>
struct TIsChar : TIsCharResolver<typename RemoveCV<TType>::Type> : TTrue {};

template<class Type>
constexpr bool IsChar = TIsChar<Type>::Value;

template <class Type> 
concept TChar = TIsChar<Type>::Value;

template <class Type> struct TIsAsciiResolver		 : TFalse {};;
template <>			  struct TIsAsciiResolver<char>  : TTrue  {};;
template <>			  struct TIsAsciiResolver<char8> : TTrue  {};;
template <>			  struct TIsAsciiResolver<ascii> : TTrue  {};;

template<class Type> 
struct TIsAscii : TIsAsciiResolver<typename RemoveCV<Type>::Type> {};

template<class Type>
constexpr bool IsAscii = TIsAscii::Value;

template<class Type> 
concept TAscii = TIsAscii<Type>::Value;

template <class Type> struct TIsWideCharResolver	  	  : TFalse {};;
template <>			  struct TIsWideCharResolver<wchar_t> : TTrue  {};;
template <>			  struct TIsWideCharResolver<wchar>   : TTrue  {};;
template <>			  struct TIsWideCharResolver<char16>  : TTrue  {};;
template <>			  struct TIsWideCharResolver<char32>  : TTrue  {};;

template<class Type> 
struct TIsWideChar : TIsWideCharResolver<typename RemoveCV<Type>::Type> {};

template<class Type>
constexpr bool IsWideChar = TIsWideChar::Value;

template<class Type> 
concept TWideChar = TIsWideChar<Type>::Value;
#pragma endregion Chars

#pragma region Floating Point
template<class Type> struct TFloatResolver : public TFalse {};

template<> struct TFloatResolver<float>			: public TTrue {};
template<> struct TFloatResolver<double>		: public TTrue {};
template<> struct TFloatResolver<long double>	: public TTrue {};

template<class TType>
struct TIsFloat : TFloatResolver<typename RemoveCV<TType>::Type> {};

template<class Type>
constexpr bool IsFloat = TIsFloat<Type>::Value;

#define LAL_Declare_FloatingPoint(Type)								\
		LAL_NamespaceStart											\
		template<> struct IsFloatResolver<Type>	: public TTrue {};	\
		LAL_NamespaceEnd
#pragma endregion Floating Point

#pragma region Arithmetic
template<class Type>
struct TIsArithmetic : public TBool<IsIntegral<Type>::Value || IsFloat<Type>::Value> {};

template<class Type>
constexpr bool IsArithmetic = TIsArithmetic<Type>::Value;
#pragma endregion Arithmetic

#pragma region Pointers
template<class TType> struct TIsNullPtr : public
	TIsSame< TRemoveCV<TType>, nullptrType> 
{};
#pragma endregion Pointers

#pragma region Consts
template<class Type> struct TIsConstResolver                       : TFalse {};
template<class Type> struct TIsConstResolver<Type const*>          : TTrue {};
template<class Type> struct TIsConstResolver<Type volatile const*> : TTrue {};

template<class Type> struct TIsConst        : TIsConstResolver<Type*> {};
template<class Type> struct TIsConst<Type&> : TFalse {};

template<class Type>
constexpr bool IsConst = TIsConst<Type>::Value;
#pragma endregion Consts

#pragma region Volatile
template<class Type> struct TIsVolatileResolver							: TFalse {};
template<class Type> struct TIsVolatileResolver<      volatile Type*>	: TTrue {};
template<class Type> struct TIsVolatileResolver<const volatile Type*>	: TTrue {};

template<class Type> struct TIsVolatile			: TIsVolatileResolver<Type*> {};
template<class Type> struct TIsVolatile<Type&>	: TFalse {};

template<class Type>
constexpr bool IsVolatile = TIsVolatile<Type>::Value;
#pragma endregion Volatile

#pragma region References
template<class Type> struct TIsReference			: TFalse {};
template<class Type> struct TIsReference<Type&>		: TTrue {};
template<class Type> struct TIsReference<Type&&>	: TTrue {};

template<class Type>
constexpr bool IsReference = TIsReference<Type>::Value;

template<class Type>
concept TReference = IsReference<Type>;

template<class Type> struct TIs_LValue_Ref			: TFalse {};
template<class Type> struct TIs_LValue_Ref<Type&>	: TTrue {};

template<class Type>
constexpr bool Is_LValue_Ref = TIs_LValue_Ref<Type>::Value;

template<class Type>
concept TLValue_Ref = Is_LValue_Ref<Type>;

template<class Type>
using TConvert_Ref_const = TAdd_LValue_Ref< TRemoveRef<Type> const>;
#pragma endregion References

#pragma region Funtions
template<class> struct TIsFunction : TFalse {};

#if Platform_Ptr_Size == 4 && defined(Platform_Microsoft) && defined(_MSC_EXTENSIONS)
static_assert(false, "Platform unsupported");

#else
template<class TReturn, class... TArgs> struct TIsFunction<TReturn (TArgs...)>      : TTrue {};
template<class TReturn, class... TArgs> struct TIsFunction<TReturn (TArgs..., ...)> : TTrue {};

template<class TReturn, class... TArgs>
constexpr bool IsFunction = TIsFunction<TReturn, TArgs...>::Value;

template<class TReturn, class... TArgs>
concept TFunction = IsFunction<TReturn, TArgs...>;
#endif
#pragma endregion Functions

#pragma region Adds
template<class TType>	struct AddReference			{ typedef TType& Type; };
template<class TType>	struct AddReference<TType&>	{ typedef TType& Type; };
template<>				struct AddReference<void>	{ typedef void   Type; };

template<class Type>
using TAddReference = AddReference<Type>::Type;

template<class TType, bool = IsConst<TType> || IsReference<TType> || IsFunction<TType>>
struct AddConstResolver
{
	typedef TType
	Type;
};

template<class TType>
struct AddConstResolver<TType, false>
{
	typedef TType const
	Type;
};

template<class TType>
using TAddConst = typename AddConstResolver<TType>::Type;

template<class TType, bool = IsVolatile<TType> || IsReference<TType> || IsFunction<TType>>
struct AddVolatile
{
	typedef TType
	Type;
};

template<class TType>
struct AddVolatile
{
	typedef TType volatile
	Type;
};

template<class TType>
using TAddVolatile = typename AddVolatile<TType>::Type;

template<class TType>
struct Add_CV
{
	typedef 
	typename AddConst<
	typename AddVolatile<TType>
			 ::Type> // Volatile
	::Type // Const

	Type;
};

template<class TType>
using TAdd_CV = typename Add_CV<TType>::Type;

template <class TType>	struct Add_LValue_Ref                      { typedef TType&              Type; }; 
template <class TType>	struct Add_LValue_Ref<TType&>              { typedef TType&		         Type; };  
template <>				struct Add_LValue_Ref<			     void> { typedef				void Type; };
template <>				struct Add_LValue_Ref<const		     void> { typedef const		    void Type; };
template <>				struct Add_LValue_Ref<	    volatile void> { typedef	   volatile void Type; };
template <>				struct Add_LValue_Ref<const volatile void> { typedef const volatile void Type; };

template <typename TType>
using TAdd_LValue_Ref = typename Add_LValue_Ref<TType>::Type;

template <class TType>	struct Add_RValue_Ref                      { typedef TType&&             Type; }; 
template <class TType>	struct Add_RValue_Ref<TType&>              { typedef TType&		         Type; };  
template <>				struct Add_RValue_Ref<			     void> { typedef				void Type; };
template <>				struct Add_RValue_Ref<const		     void> { typedef const		    void Type; };
template <>				struct Add_RValue_Ref<	    volatile void> { typedef	   volatile void Type; };
template <>				struct Add_RValue_Ref<const volatile void> { typedef const volatile void Type; };

template <class TType>
using TAdd_RValue_Ref = typename Add_RValue_Ref<TType>::Type;

// declval
template<class Type>
typename TAdd_RValue_Ref<Type> declval();
#pragma endregion Adds

#pragma region ResultOf
template<class> struct TResultOf_Resolver;

template<class First, class... TypeArgs>
struct TResultOf_Resolver<First(TypeArgs...)> {
	typedef decltype(ToReference<First>()(ToReference<ArgTypes>()...))
	Type;
};

template<class TType>
using TResultOf = typename TResultOf_Resolver<TType>::Type;
#pragma endregion ResultOf

#pragma region Unions
#if Compiler_Intrinsic_TypeTraits_Supported										\
&& (defined(Compiler_MSVC) || defined(Compiler_GCC) || (defined(Compiler_Clang) \
&& Compiler_Has_Feature(is_union)))
template <typename T> 
struct TIsUnion : public TIsBool<__is_union(Type)>{};
#else
static_assert(false, "LAL: Compiler must support is_union type trait.");
#endif

#define LAL_Declare_Union(Type)											\
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
static_assert(false, "LAL: Compiler must support is_class type trait.");
#endif

template<class Type>
constexpr bool IsClass = TIsClass<Type>::Value;
#pragma endregion Classes

#pragma region Enums
#if Compiler_Intrinsic_TypeTraits_Supported									\
&& (defined(_MSC_VER) || defined(Compiler_GCC) || (defined(Compiler_Clang)	\
&& Compiler_Has_Feature(is_enum)))
template <typename Type> 
struct TIsEnum : public TBool<__is_enum(Type)>{};

#else
static_assert(false, "Compiler must support is_enum type trait.");
#endif

template<class Type>
constexpr bool IsEnum = TIsEnum<Type>::Value;

#define LAL_Declare_Enum(Type)											\
		LAL_NamespaceStart												\
		template <> struct TIsEnum<      Type> : public true_type{};	\
		template <> struct TIsEnum<const Type> : public true_type{};	\
		LAL_NamespaceEnd
#pragma endregion Enum

#pragma region Empty
#if Compiler_Intrinsic_TypeTraits_Supported && \
(defined(Compiler_MSVC) || defined(Compiler_GNU) || (defined(Compiler_Clang) && Compiler_Has_Feature(is_empty)))
template<class Type> 
struct TIsEmpty : TBool<__is_empty(Type)> {};

#else
static_assert(false, "Compiler must support is_empty type trait.");
#endif

template<class Type>
constexpr bool IsEmpty = TIsEmpty<Type>::Value;
#pragma endregion Empty

#pragma region Constructible
#if Compiler_Intrinsic_TypeTraits_Supported && \
(defined(Compiler_MSVC) || defined(Compiler_GNU) || (defined(Compiler_Clang) && Compiler_Has_Feature(is_constructible)))
template<class Type>
struct TIsConstructible : TBool<__is_constructible(Type)> {};

#else
static_assert(false, "Compiler must support is_constructible type trait.");
#endif

template<class Type>
constexpr bool IsConstructible = TIsConstructible<Type>::Value;
#pragma endregion Constructible

#pragma region IntegerSequence
template<TIntegral TType, TType... Ints>
struct TIntSequence
{
	typedef TType
	Type;

	constexpr static sw Size()
	{
		return sizeof...(Ints);
	}
};

namespace IndexSequence {

template<sw Num, class IndexSequence> struct Make;

// Base case
template<sw... Ints>
struct Make<0, TIntSequence<sw, Ints...>>
{
	typedef IntSequence<sw, Ints...> 
	Type;
};

// N-th case
template<sw Num, sw... Ints>
struct Make<Num, TIntSequence<sw, Ints...>>
{
	typedef typename Make<Num - 1, IntSequence<sw, Num - 1, Ints...>>::Type
	Type;
};

template<sw Num>
using TMake = typename Make<Num, TIntSequence<sw>>::Type;

template<class... Type>
using For = TMake<sizeof...(Type)>;
} // Index Sequence

template<sw... Ints>
using TIndexSequence = IntSequence<sw, Ints...>;

namespace IntSequence {

template<class Target, class Sequence>
struct Convert;

template<class Target, sw... Ints>
struct Convert<Target, TIntSequence<sw, Ints...>>
{
	typedef IntSequence<Target, Ints...>
	Type;
};

template<class Type, sw Num>
struct Make
{
	typedef typename Convert<Type, IndexSequence::TMake<Num>>::Type
	Type;
};

template<class Type, sw Num>
using TMake = typename Make<Type, Num>::Type;
} // IntSequence
#pragma endregion IntegerSequence

LAL_NamespaceEnd

