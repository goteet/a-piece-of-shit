#pragma once
#include "cxx_vector2.h"
#include "cxx_base_point.h"

namespace cxx
{
	template<typename E>
	struct point_t<E, dimension::_2> : public vector_t<E, dimension::_2>
	{
		constexpr point_t() = default;

		constexpr point_t(const point_t&) = default;

		constexpr point_t(const vector_t<E, dimension::_2>& rhs) : vector_t<E, dimension::_2>(rhs) { };

		constexpr point_t(E x, E y) : vector_t<E, dimension::_2>(x, y) { }

		static constexpr point_t origin()
		{
			return point_t(constants<E>::zero, constants<E>::zero);
		}
	};

	template<typename E, dimension D>
	constexpr point_t<E, D> operator+ (
		const point_t<E, D>& lhs,
		const point_t<E, D>& rhs)
	{
		return static_cast<const vector_t<E, D>>(lhs)
			+ static_cast<const vector_t<E, D>>(rhs);
	}

	template<typename E, dimension D>
	constexpr point_t<E, D> operator* (const point_t<E, D>& lhs, const scaler_t<E> scaler)
	{
		return static_cast<const vector_t<E, dimension::_2>>(lhs) * scaler;
	}

	template<typename E, dimension D>
	constexpr point_t<E, D> operator* (const scaler_t<E> scaler, const point_t<E, D>& rhs)
	{
		return static_cast<const vector_t<E, dimension::_2>>(rhs) * scaler;
	}
}