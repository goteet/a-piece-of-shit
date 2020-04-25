#pragma once
#include "cxx_mathcommon.h"
#include "cxx_mathutility.h"
namespace cxx
{
	template<typename E> struct radian;
	template<typename E> struct degree;

	using degree_i = degree<int>;
	using radian_f = radian<float>;
	using degree_f = degree<float>;
	using radian_d = radian<double>;
	using degree_d = degree<double>;

	namespace angle_impl
	{
		template<class E>
		constexpr E r2d_factor = E(180) / constants<E>::pi;

		template<class E>
		constexpr E d2r_factor = constants<E>::pi / E(180);
	}
}
