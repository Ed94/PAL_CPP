#pragma once
#include "LAL.Config.hpp"
#include "LAL.Metaprogramming.hpp"
#include "LAL.Types.hpp"

LAL_NamespaceStart

template<class Type_>
struct RefWrap
{
	using Type = Type_;

	RefWrap(Type&) noexcept;
	RefWrap(Type&&);
	RefWrap(RefWrap<Type> const&) noexcept;

	Type& Get() const noexcept;

	RefWrap& operator = (RefWrap<Type> const& ref) noexcept;

	operator Type& () const noexcept;

	template<class... TypeArgs>
	TResultOf<Type& (TypeArgs&&...)> operator() (TypeArgs&&...) const;

protected:
	Type* ptr;
};

#pragma region Implementation
#define RefWrapSym		template<class Type> RefWrap<Type>

RefWrapSym::RefWrap(Type& ref)					: ptr(addressof(ref)) {}
RefWrapSym::RefWrap(RefWrap<Type> const& other) : ptr(other.ptr) {}
//RefWrapSym::RefWrap()

#undef RefWrapSym
#pragma endregion Implementation

LAL_NamespaceEnd
