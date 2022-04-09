#pragma once

#include "LAL.Config.hpp"
#include "LAL.Metaprogramming.hpp"
#include "LAL.Types.hpp"
#include "LAL.Utility.hpp"

// This is a perfect example of why template (definiton expansion based) metaprogramming is awful.

/*
Non-recursive tuple implementation based on EASTL which is related to:
http://mitchnull.blogspot.ca/2012/06/c11-tuple-implementation-details-part-1.html
*/

LAL_NamespaceStart

template<class... Types> struct TTuple;

namespace Tuple {

template<		   class Tuple> struct Size;
template<sw Index, class Tuple> struct Element;

#pragma region Boilerplate
template<sw Index, class Tuple> 
using TElement = typename Element<Index, Tuple>::Type;

// const variant for TElement; Needed when tuple cannot itself be const
template<sw Index, class Tuple>
using TElement_const = SelectIf<Is_LValue_Ref< TElement<Index, Tuple>>,
	// if True: Convert to ref const
		TConvertTo_Ref_const<TElement<Index, Tuple>>,
	// else: Otherwise apply const directly
		TElement<Index, Tuple> const
>;
#pragma endregion BoilerPlate
} // Tuple

template<sw Index, class ... TArgs> using TElement       = Tuple::TElement<Index, TTuple<TArgs...>>;
template<sw Index, class ... TArgs> using TElement_const = Tuple::TElement_const<Index, TTuple<TArgs...>>;

#pragma region Unrollers
template<sw Index,   class... TArgs>	TElement<Index, TArgs...>&			Get(TTuple<TArgs...>& tuple);
template<sw Index,   class... TArgs>	TElement<Index, TArgs...>&&			Get(TTuple<TArgs...>&& tuple);
template<sw Index,   class... TArgs>	TElement_const<Index, TArgs...>&	Get(TTuple<TArgs...> const& tuple);
template<class Type, class... TArgs>	Type&								Get(TTuple<TArgs...>& tuple);
template<class Type, class... TArgs>	Type const&							Get(TTuple<TArgs...> const& tuple);
template<class Type, class... TArgs>	Type&&								Get(TTuple<TArgs...>&& tuple);
#pragma endregion Unrollers

namespace Tuple {
template<class Indices, class... TArgs> struct Impl;

#pragma region Size
template<class Type> class Size				         {};
template<class Type> class Size<const          Type> {};
template<class Type> class Size<      volatile Type> {};
template<class Type> class Size<const volatile Type> {};

template<               class... TArgs> struct Size< ArgTypes<TArgs...>>      : TConstant<sw, sizeof...(TArgs)> {};
template<               class... TArgs> struct Size< TTuple  <TArgs...>>      : TConstant<sw, sizeof...(TArgs)> {};
template<class Indices, class... TArgs> struct Size< Impl<Indices, TArgs...>> : TConstant<sw, sizeof...(TArgs)> {};
#pragma endregion Size

#pragma region Element
template<sw Index, class Type> struct Element {};

// Out of range condition
template<sw Index> 
struct Element<Index, ArgTypes<>>
{
	static_assert(Index != Index, "Element index is out of range.");
};

// Base Case
template<class ElemType, class... TArgs>
struct Element<0, ArgTypes<ElemType, TArgs...>>
{
	typedef ElmType
	Type;
};

// N-th Case
template<sw Index, class ElemType, class... TArgs>
struct Element<Index, ArgTypes<ElemType, TArgs...>>
{
	typedef TElement<Index - 1, ArgTypes<TArgs...>>
	Type;
};

#pragma region TTuple Specialization
template<sw Index, class... TArgs>
struct Element<Index, TTuple<TArgs...>>
{
	typedef TElement<Index, ArgTypes<TArgs...>> 
	Type;
};

template<sw Index, class... TArgs>
struct Element<Index, TTuple<TArgs...> const>
{
	typedef TAddConst< TElement<Index, ArgTypes<TArgs...>>> 
	Type;
};

template<sw Index, class... TArgs>
struct Element<Index, TTuple<TArgs...> volatile>
{
	typedef TAddVolatile< TElement<Index, ArgTypes<TArgs...>>>
	Type;
};

template<sw Index, class... TArgs>
struct Element<Index, TTuple<TArgs...> const volatile>
{
	typedef TAdd_CV< TElement<Index, ArgTypes<TArgs...>>>
	Type;
};
#pragma endregion TTuple Specialization

#pragma region Impl Specialization
template<sw Index, class Indices, class... TArgs>
struct Element<Index, Impl<Indices, TArgs...>> :  Element<Index, TTuple<TArgs...>>
{};

template<sw Index, class Indices, class... TArgs>
struct Element<Index, Impl<Indices, TArgs...> const> : Element<Index, TTuple<TArgs...> const>
{};

template<sw Index, class Indices, class... TArgs>
struct Element<Index, Impl<Indices, TArgs...> volatile> : Element<Index, TTuple<TArgs...> volatile>
{};

template<sw Index, class Indices, class... TArgs>
struct Element<Index, Impl<Indices, TArgs...> const volatile> : Element<Index, TTuple<TArgs...> const volatile>
{};
#pragma endregion Impl Specialization
#pragma endregion Element

#pragma region Index
template<class Type, class Tuple> 
struct Index {};

// Base Case
template<class Type>
struct Index<Type, ArgTypes<>>
{
	typedef void
	DuplicateTypeCheck;

	static sw const Value = 0;

	Index() == delete; // Index should only be used for compile-time assistance, and never be instantiated
};

// Rest
template<class Type, class... TArgs>
struct Index<Type, ArgTypes<Type, TArgs...>>
{
	typedef int
	DuplicateTypeCheck;

	// After finding type in the list of types, try to find type Type in TArgs.
	// If we stumble back into this version of Index, i.e. Type appears twice in the list of types, 
	// then DuplicateTypeCheck will be of type int, and the static_assert will fail.
	// If we don't, then we'll go through the version of Index above, 
	// where all of the types have been exhausted, and DuplicateTypeCheck will be void.
	static_assert(
		IsVoid<typename Index<Type, ArgTypes<TArgs...>>::DuplicateTypeCheck>, 
		"Duplicate type T in ::get<T>(); unique types must be provided in declaration, or only use Get<sw>()");

	static sw const Value = 0;
};

template<class Type, class TArgsHead, class... TArgs>
struct Index< Type, ArgTypes<TArgsHead, TArgs...>>
{
	typedef typename Index<Type, ArgTypes<TArgs...>>::DuplicateTypeCHeck
	DuplicateTypeCheck;

	static const sw Value = Index<Type, ArgTypes<TArgs...>>::Value + 1;
};

template<class Type, class Indices, class... TArgs>
struct Index<Type, Impl<Indices, TArgs...>> : Index<Type, ArgTypes<TArgs...>> {};
#pragma endregion Index

#pragma region Leaf
template<sw Index, class Type, bool IsValueEmpty = IsEmpty<Type>> struct Leaf;

template<sw Index, class Type, bool IsEmpty> inline void
swap(Leaf<Index, Type, IsEmpty>& a, Leaf<Index, Type, IsEmpty>& b)
{
	::swap(a.GetInternal(), b.GetInternal());
}

// Base Case
template<sw Index, class Type, bool IsEmpty>
struct Leaf
{
	Leaf() : mValue() {}
	Leaf(Leaf const&) = default;

	explicit Leaf(Type&& value) 
		: mValue(move(value)) 
	{};

	template<class OtherType, typename = EnableIf<IsConstructible<Type>, OtherType&&>>
	explicit Leaf(OtherType&& value) 
		: mValue(forward<OtherType>(value)) 
	{}

	template<class OtherType>
	explicit Leaf(Leaf<Index, OtherType>& value) 
		: mValue(value.GetInternal()) 
	{}

	Type& GetInternal() {
		return mValue;
	}

	Type const& GetInternal() const {
		return mValue;
	}

	int swap(Leaf& value)
	{
		Tuple::swap(dref this, value);
		return 0;
	}

	Leaf& operator = (Leaf const&) = delegate;

	template<class OtherType>
	Leaf& operator = (OtherType&& value)
	{
		mValue = forward<OtherType>(value);
		return dref this;
	}

protected:
	Type mValue;
};

// Reference Specialization
template<sw Index, class Type, bool IsEmpty>
struct Leaf<Index, Type&, IsEmpty>
{
	Leaf(Leaf const&) = default;

	explicit Leaf(Type& value) 
		: mValue(value) 
	{}
	explicit Leaf(Type&& value) 
		: mValue(move(value)) 
	{}

	template<class OtherType, typename = EnableIf<IsConstructible<Type>, OtherType&&>>
	explicit Leaf(OtherType&& value)
		: mValue(forward<OtherType>(value)) 
	{}

	template<class OtherType>
	explicit Leaf(Leaf<Index, OtherType> const& value) 
		: mValue(value.GetInternal()) 
	{}

	Type& GetInternal() {
		return mValue;
	}

	Type const& GetInternal() const {
		return mValue;
	}

	int swap(Leaf& value)
	{
		Tuple::swap(dref this, value);
		return 0;
	}

	Leaf& operator = (Leaf const&) = delegate;

	template<class OtherType>
	Leaf& operator = (OtherType&& value)
	{
		mValue = forward<OtherType>(value);
		return dref this;
	}

protected:
	Type& mValue;
};

// Partial Specialization: Empty Base Class Optimization
template<sw Index, class Type>
struct Leaf<Index, Type, true> : private Type
{
	Leaf(Leaf const&) = default;

	template<class OtherType, typename = EnableIf<IsConstructible<Type>, OtherType&&>>
	explicit Leaf(OtherType&& value)
		: Type(forward<OtherType>(value))
	{}

	template<class OtherType>
	explicit Leaf(Leaf<Index, OtherType>& value)
		: Type(value.GetInternal())
	{}

	Type& GetInternal() {
		return scast<Type&(dref this);
	}

	Type const& GetInternal() const {
		return scast<Type&>(dref this);
	}

	int swap(Leaf& value)
	{
		Tuple::swap(dref this, value);
		return 0;
	}

	template<class OtherType>
	Leaf& operator = (OtherType&& value)
	{
		Type::operator=(forward<OtherType>(value));
		return dref this;
	}

protected:
	Leaf& operator = (Leaf const&) = delete;
};
#pragma endregion Leaf

#pragma region MakeTypes
template<class Types, class Tuple, sw Start, sw End> struct MakeTypes;

template<class... Types, class Tuple, sw Start, sw End> 
struct MakeTypes<ArgTypes<Types...>, Tuple, Start, End>
{
	typedef TRemoveRef<Tuple>
	TupleType;

	typedef 
	typename MakeTypes<
				ArgTypes<Types..., SelectIf<Is_LValue_Ref<Tuple>,
				// True: Append ref
					TElement<Start, TupleType>&,
				// False: Append non-ref
					TElement<Start, TupleType>>>,
				Tuple, Start + 1, End
			>
	::Type // MakeTypes's Type
	Type;
};

template<class... Types, class Tuple, sw End>
struct MakeTypes<ArgTypes<Types...>, Tuple, End, End>
{
	typedef ArgTypes<Types...> 
	Type;
};

template<class Tuple>
using TMakeTypes = typename MakeTypes<ArgTypes<>, Tuple, 0, Size<TRemoveRef<Tuple>>::Type;
#pragma endregion MakeTypes

#pragma region Impl
template<sw Index, class Indices, class... TArgs>	TElement<Index, Impl<Indices, TArgs...>>&&		Get(Impl<Indices, TArgs...>& value);
template<sw Index, class Indices, class... TArgs>	TElement_const<Index, Impl<Indices, TArgs...>>&	Get(Impl<Indices, TArgs...> const& value);
template<sw Index, class Indices, class... TArgs>	TElement<Index, Impl<Indices, TArgs...>>&&		Get(Impl<Indices, TArgs...>&& value);

template<class Type, class Indices, class... TArgs>	Type&		Get(Impl<Indices, TArgs...>& value);
template<class Type, class Indices, class... TArgs> Type const& Get(Impl<Indices, TArgs...> const& value);
template<class Type, class Indices, class... TArgs>	Type&&		Get(Impl<Indices, TArgs...>&& value);

// feels bad man.
template<sw... Indices, class... TArgs>
struct Impl< TIntSequence<sw, Indices...>, TArgs...> : Leaf<Indices, TArgs>...
{
	constexpr Impl() = default;

	// index_sequence changed to integer_sequence due to issues described below in VS2015 CTP 6. 
	// https://connect.microsoft.com/VisualStudio/feedback/details/1126958/error-in-template-parameter-pack-expansion-of-std-index-sequence
	// 
	template<class... TArgs, class... ValueTypes>
	explicit Impl(IntSequence<sw, Indices...>, ArgTypes<TArgs...>, ValueTypes&&... values)
		: Leaf<Indices, TArgs>(forward<ValueTypes>(values))...
	{}

	template<class OtherTuple>
	Impl(OtherTuple&& tuple)
		: Leaf<Indices, TArgs>(
			forward< TElement<Indices, TMakeTypes<OtherTuple>>>(Get<Indices>(tuple))
		)...
	{}

	void swap(Impl& tuple)
	{
		swallow(Leaf<Indices, TArgs>::swap(
			scast<Leaf<Indices, TArgs>&>(tuple))
		...);
		return dref this;
	}

	template<class OtherTuple>
	Impl& operator=(OtherTuple&& tuple)
	{
		swallow(Leaf<Indices, TArgs>::operator=(
			forward< TElement<Indices, TMakeTypes<OtherTuple>>>(Get<Indices>(tuple))
		...);

		return dref this;
	}

	Impl& operator= (const TmpleImpl& tuple)
	{
		swallow(Leaf<Indices, TArgs>::operator=(
			scast<Leaf<Indices, TArgs> const&>(tuple).GetInternal())
		...);

		return dref this;
	}
};

template<sw Index, class Indices, class... TArgs> 
inline TElement<Index, Impl<Indices, TArgs...>>&
Get(Impl<Indices, TArgs...>& tuple)
{
	typedef TElement<Index, Impl<Indices, TArgs...>> 
	Type;
	return scast<Leaf<Index, Type>&>(tuple).GetInternal();
}

template<sw Index, class Indices, class... TArgs>
inline TElement_const<Index, Impl<Indices, TArgs...>>&
Get(Impl<Indices, TArgs...> const& tuple)
{
	typedef TElement<Index, Impl<Indices, TArgs...>>
	Type;
	return scast<Leaf<Index, Type> const&>(tuple).GetInternal();
}

template<sw Index, class Indices, class... TArgs>
inline TElement_const<Index, Impl<Indices, TArgs...>>&&
Get(Impl<Indices, TArgs...>&& tuple)
{
	typedef TElement<Index, Impl<Indices, TArgs...>>
	Type;
	return scast<Type&&>(scast<Leaf<Index, Type>&>(tuple).GetInternal());
}

template<class Type, class Indices, class... TArgs>
inline Type&
Get(Impl<Indices, TArgs...>& tuple)
{
	typedef Index<Type, Impl<Indices, TArgs...>>
	Index;
	return scast<Leaf<Index::Value, Type>&>(tuple).GetInternal();
}

template<class Type, class Indices, class... TArgs>
inline Type const&
Get(Impl<Indices, TArgs...> const& tuple)
{
	typedef Index<Type, Impl<Indices, TArgs...>>
	Index;
	return scast<Type&&>(scast<Leaf<Index::Value, Type>&>(tuple).GetInternal());
}
#pragma endregion Impl

#pragma region Traits

#pragma endregion Traits
} // Tuple

#pragma region TTuple
template<class Type, class... TArgs>
struct TTuple<Type, TArgs...>
{
		

protected:
	typedef Tuple::Impl<IndexSequence::TMake<sizeof...(TArgs) + 1>, Type, TArgs...> 
	Impl;
	Impl mImpl;
};
#pragma endregion TTuple
LAL_NamespaceEnd

