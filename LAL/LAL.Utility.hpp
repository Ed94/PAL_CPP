#pragma once

#include "LAL.Config.hpp"
#include "LAL.Casting.hpp"
#include "LAL.Metaprogramming.hpp"

// Ref: EASTL

LAL_NamespaceStart

// forward
//
// forwards the argument to another function exactly as it was passed to the calling function.
// Not to be confused with move, this is specifically for echoing templated argument types
// to another function. move is specifically about making a type be an rvalue reference (i.e. movable) type.
template<class Type> 
constexpr Type&& 
forward(TRemoveRef<Type>& ref) noexcept
{
	return scast<Type&&>(ref);
}

template<TLValue_Ref Type> 
constexpr Type&& 
forward(TRemoveRef<Type>&& rvalue) noexcept
{
	return scast<Type&&>(rvalue);
}

// move
//
// move obtains an rvalue reference to its argument and converts it to an xvalue.
// Returns, by definition: static_cast<typename remove_reference<T>::type&&>(t).
// The primary use of this is to pass a move'd type to a function which takes T&&,
// and thus select that function instead of (e.g.) a function which takes T or T&.
// See the C++ Standard, section 20.2.3
// http://en.cppreference.com/w/cpp/utility/move
template<class Type>
constexpr TRemoveRef<Type>&&
move(Type&& rvalue) noexcept
{
	return scast<TRemoveRef<Type>&&>(rvalue);
}

template<class Type>
constexpr Type&&
move_if_noexcept(Type& ref) noexcept
{
	return move<Type>(ref);
}

/*
swap

Assigns the contents of a to b and the contents of b to a. 
A temporary instance of type T is created and destroyed
in the process.

This function is used by numerous other algorithms, and as 
such it may in some cases be feasible and useful for the user 
to implement an override version of this function which is 
more efficient in some way. 
*/
template<class Type> inline void
swap(Type& a, Type& b)
{
	Type temp(move(a));
	a = move(b);
	b = move(temp);
}

LAL_NamespaceEnd
