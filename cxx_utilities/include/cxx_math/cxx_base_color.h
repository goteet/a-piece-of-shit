#pragma once
#include "cxx_mathutility.h"
#include "cxx_mathcommon.h"
#include "cxx_vector.h"

namespace cxx
{
	template<typename E, dimension D>
	struct color_t;

	template<typename E> using color3 = color_t<E, dimension::_3>;
	template<typename E> using color4 = color_t<E, dimension::_4>;

	using color3f = color3<float>;
	using color4f = color4<float>;
	using color3d = color3<double>;
	using color4d = color4<double>;
	using color3b = color3<unsigned char>;
	using color4b = color4<unsigned char>;
	using colordw = unsigned int;

	namespace color_impl
	{
		template<class E>
		constexpr E u2f_factor = constants<E>::one / E(255);

		template<class E>
		constexpr E r2f_factor = constants<E>::one / E(RAND_MAX);

		constexpr double r2u_factor = 255.0 / RAND_MAX;

		template<typename E>
		constexpr unsigned char to_255(E v)
		{
			return static_cast<unsigned char>(
				clamp<int>(v * 255, 0, 255)
				);
		}

		constexpr unsigned int bgra(
			unsigned char b,
			unsigned char g,
			unsigned char r,
			unsigned char a)
		{
			return (b | (g << 8) | (r << 16) | (a << 24));
		}

		constexpr unsigned int rgba(
			unsigned char r,
			unsigned char g,
			unsigned char b,
			unsigned char a)
		{
			return (r | (g << 8) | (b << 16) | (a << 24));
		}
	}

	// operators
	template<typename E, dimension D>
	constexpr bool operator==(const color_t<E, D>& lhs, const color_t<E, D>& rhs) { return lhs.value == rhs.value; }

	template<typename E, dimension D>
	constexpr bool operator!=(const color_t<E, D>& lhs, const color_t<E, D>& rhs) { return !(lhs == rhs); }

	template<typename E, dimension D>
	constexpr color_t<E, D> operator+(const color_t<E, D>& lhs, const color_t<E, D>& rhs) { return color_t<E, D>(lhs.value + rhs.value); }

	template<typename E, dimension D>
	constexpr color_t<E, D> operator*(const color_t<E, D>& lhs, const color_t<E, D>& rhs) { return color_t<E, D>(lhs.value * rhs.value); }

	template<typename E, dimension D>
	constexpr color_t<E, D> operator*(const color_t<E, D>& lhs, scaler_t<E> rhs) { return color_t<E, D>(lhs.value * rhs); }

	template<typename E, dimension D>
	constexpr color_t<E, D> operator*(scaler_t<E> lhs, const color_t<E, D>& rhs) { return rhs * lhs; }

	template<typename E, dimension D>
	constexpr color_t<E, D>& operator+=(color_t<E, D>& lhs, const color_t<E, D>& rhs) { lhs.value += rhs.value; return lhs; }

	template<typename E, dimension D>
	constexpr color_t<E, D>& operator*=(color_t<E, D>& lhs, const color_t<E, D>& rhs) { lhs.value *= rhs.value; return lhs; }

	template<typename E, dimension D>
	constexpr color_t<E, D>& operator*=(color_t<E, D>& lhs, scaler_t<E> rhs) { lhs.value *= rhs; return lhs; }
}