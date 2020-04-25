#pragma once
#include "cxx_radian.h"
#include "cxx_vector3.h"

namespace cxx
{
	template<typename E> struct euler;

	using euler_f = euler<float>;
	using euler_d = euler<double>;


	template<typename E>
	struct euler
	{
		radian<E> roll;
		radian<E> pitch;
		radian<E> yaw;

		constexpr euler() = default;

		constexpr euler(const radian<E>& _roll, const radian<E>& _pitch, const radian<E>& _yaw)
			: roll(_roll)
			, pitch(_pitch)
			, yaw(_yaw) { }

		constexpr euler(const vector_t<E, dimension::_3>& rpy)
			: roll(rpy.x)
			, pitch(rpy.y)
			, yaw(rpy.z) { }
	};


	template<typename E>
	constexpr bool operator== (const euler<E>& lhs, const euler<E>& rhs)
	{
		return &lhs == &rhs
			|| (lhs.roll == rhs.roll &&
				lhs.pitch == rhs.pitch &&
				lhs.yaw == rhs.yaw);
	}

	template<typename E>
	constexpr bool operator!= (const euler<E>& lhs, const euler<E>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_3> to_vector3(const euler<E>& e)
	{
		return vector_t<E, dimension::_3>(
			e.roll.value,
			e.pitch.value,
			e.yaw.value
			);
	}
}