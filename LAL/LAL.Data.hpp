#pragma once

#include "LAL.Config.hpp"
#include "LAL.RefWrap.hpp"
#include "LAL.Types.hpp"

/*
Data Related Utilities

Currently has some study of ways to deal with data layouts (SOA AOS interchange)
*/


LAL_NamespaceStart

#pragma region Crosseto's
enum class
EDataLayout
{
	AOS,	// Array of structures
	SOA,	// Structure of arrays
};

constexpr EDataLayout AOS = EDataLayout::AOS;
constexpr EDataLayout SOA = EDataLayout::SOA;

template<typename TContainer> class Iter;

template<
	template<typename...> class Container,
	EDataLayout Layout, 
	class TElement>
struct DataLayoutPolicy;

template<
	template<typename...> class Container,
	template<typename...> class TElement,
	typename... Types>
struct DataLayoutPolicy
	<Container, EDataLayout::AOS, TElement<Types...>>
{
	using Type		= Container<TElement<Types...>>;
	using ValueType = TElement<Types...>&;

	constexpr static ValueType
	Get(Type& container, sw pos)
	{
		return ValueType(dref scast < p<TElement<Types...>>(ptrof container[position]));
	}

	template<class TValue>
	constexpr static void
	PushBack(Type& container, TValue&& value)
	{
		container.PushBack(value);
	}

	constexpr static void
	Resize(Type& container, sw newSize)
	{
		container.Resize(newSize);
	}

	constexpr static sw
	Size(Type& container)
	{
		return container.Size;
	}
};


template <template <typename...> class Container, template<typename...> class TElement, typename... Types>
struct DataLayoutPolicy
	<Container, EDataLayout::SOA, TElement<Types...>> 
{
	using Type      = TTuple<Container<Types>...>;
	using ValueType = TElement<RefWrap<Types>...>;

	constexpr static 
	ValueType Get(Type& container, sw pos)
	{
		return doGet( container, position_, std::make_integer_sequence<unsigned, sizeof...( Types )>() ); // unrolling parameter pack
	}

	constexpr static void resize( type& c_, std::size_t size_ )	{
		doResize( c_, size_, std::make_integer_sequence<unsigned, sizeof...( Types )>() ); // unrolling parameter pack
	}

	template <typename TValue>
	constexpr static void push_back( type& c_, TValue&& val_ ){
		doPushBack( c_, std::forward<TValue>(val_), std::make_integer_sequence<unsigned, sizeof...( Types )>() ); // unrolling parameter pack
	}

	static constexpr std::size_t size(type& c_){ return std::get<0>( c_ ).size(); }

private:

	template <unsigned... Ids>
	constexpr static auto doGet( type& c_, std::size_t position_, std::integer_sequence<unsigned, Ids...> )
	{
		return value_type{ ref_wrap( std::get<Ids>( c_ )[ position_ ] )... }; // guaranteed copy elision
	}

	template <unsigned... Ids>
	constexpr static void doResize( type& c_, unsigned size_, std::integer_sequence<unsigned, Ids...> )
	{
		( std::get<Ids>( c_ ).resize( size_ ), ... ); //fold expressions
	}

	template <typename TValue, unsigned... Ids>
	constexpr static void doPushBack( type& c_, TValue&& val_, std::integer_sequence<unsigned, Ids...> )
	{
		( std::get<Ids>( c_ ).push_back( std::get<Ids>( std::forward<TValue>( val_ ) ) ), ... ); // fold expressions
	}
};
#pragma endregion Crosseto's

LAL_NamespaceEnd
