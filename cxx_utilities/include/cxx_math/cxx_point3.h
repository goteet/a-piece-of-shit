#pragma once
#include "cxx_vector3.h"
#include "cxx_point2.h"

namespace cxx
{
	template<typename E>
	struct point_t<E, dimension::_3> : public vector_t<E, dimension::_3>
	{
		constexpr point_t() = default;

		constexpr point_t(const point_t&) = default;

		constexpr point_t(const vector_t<E, dimension::_3>& rhs) : vector_t<E, dimension::_3>(rhs) { };

		constexpr point_t(E x, E y, E z) : vector_t<E, dimension::_3>(x, y, z) { }

		constexpr point_t(const vector_t<E, dimension::_2>& rhs, scaler_t<E> z) : vector_t<E, dimension::_3>(rhs, z) { };

		static constexpr point_t origin()
		{
			return point_t(constants<E>::zero, constants<E>::zero, constants<E>::zero);
		}
	};
}