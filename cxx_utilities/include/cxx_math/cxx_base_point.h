#pragma once
#include "cxx_vector.h"

namespace cxx
{
	template<typename E, dimension D>
	struct point_t;

	template<typename E>
	using point2d = point_t<E, dimension::_2>;

	template<typename E>
	using point3d = point_t<E, dimension::_3>;

	using point2d_i = point2d<int>;
	using point3d_i = point3d<int>;
	using point2d_f = point2d<float>;
	using point3d_f = point3d<float>;
	using point2d_d = point2d<double>;
	using point3d_d = point3d<double>;

	template<typename E, dimension D>
	constexpr point_t<E, D> operator+ (
		const point_t<E, D>& lhs,
		const point_t<E, D>& rhs);

	template<typename E, dimension D>
	constexpr point_t<E, D> operator* (const point_t<E, D>& lhs, const scaler_t<E> scaler);

	template<typename E, dimension D>
	constexpr point_t<E, D> operator* (const scaler_t<E> scaler, const point_t<E, D>& rhs);
}