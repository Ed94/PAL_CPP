#pragma once
#ifndef HPP_LAL_Casting

#define ocast(Type_, Obj_)	(*(Type_*))(&Obj_)

#define ccast	const_cast
#define dcast	dynamic_cast
#define rcast	reinterpret_cast
#define scast	static_cast

#define HPP_LAL_Casting
#endif