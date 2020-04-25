#pragma once
#include "cxx_mathcommon.h"
#include "cxx_vector.h"
#include "cxx_point.h"

namespace cxx
{
	template<typename E, dimension D>
	struct ray_t;

	template<typename E> using ray2d = ray_t<E, dimension::_2>;
	template<typename E> using ray3d = ray_t<E, dimension::_3>;

	using ray2d_f = ray2d<float>;
	using ray3d_f = ray3d<float>;
	using ray2d_d = ray2d<double>;
	using ray3d_d = ray3d<double>;

	template<typename E, dimension D>
	struct ray_t
	{
		using vector = vector_t<E, D>;
		using point = point_t<E, D>;
		using nvector = normalized_vector_t<E, D>;

		ray_t() = default;

		ray_t(const point& o, const nvector& d)
			: m_origin(o)
			, m_direction(d)
		{ }

		constexpr const point& origin() const { return m_origin; }

		constexpr const nvector& direction() const { return m_direction; }

		constexpr void set_origin(const point& o) { m_origin = o; }

		constexpr void set_direction(const nvector& dir) { m_direction = dir; }

		constexpr point get_offset(E offset) const { return m_direction * offset; }

	private:
		point m_origin;
		nvector m_direction;
	};
}